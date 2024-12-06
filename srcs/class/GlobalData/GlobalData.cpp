#include "GlobalData.hpp"
#include "../Client/Client.hpp"
#include "../RawBits/RawBits.hpp"
#include "../Parser/Parser.hpp"
#include "../../../includes/includes.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/select.h>
#include <vector>

bool g_running = true;

GlobalData::GlobalData() { 
	_epoll_fd = -1;
}

GlobalData::~GlobalData() { 
	if (_epoll_fd != -1)
		close(_epoll_fd);
}

void GlobalData::addToEpoll(int fd, uint32_t events)
{
	struct epoll_event ev;

	ev.events = events;
	ev.data.fd = fd;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Setting flags with fcntl failed");
	}
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		throw std::runtime_error("Epoll add error");
	}
}

void GlobalData::initServers(std::vector<Server> &servVec) {
	std::vector<Server>::iterator servIt = servVec.begin();
	std::vector<Server>::iterator end = servVec.end();

	_epoll_fd = epoll_create1(0);
	if (_epoll_fd == -1)
		throw std::runtime_error("epoll_create1 failed !");
	while (servIt != end) {
		servIt->init();
		int servFd = servIt->getSocketFd();
		addToEpoll(servFd, EPOLLIN);
		_servMap[servFd] = *servIt;
		servIt++;
	}
}

int GlobalData::waitFdsToBeReady(void) {
	std::map<int, Server>::iterator it = _servMap.begin();
	std::map<int, Server>::iterator end = _servMap.end();
	
	while (it != end)
		(it++)->second.checkCgi();
	return epoll_wait(_epoll_fd, _events, MAX_EVENTS, 1);
}

void GlobalData::addNewClient(Server &server) {
	Client *client;
	int clientFd;
	struct sockaddr_in client_addr;
	unsigned int socklen = sizeof(client_addr);

	clientFd = accept(server.getSocketFd(),  (struct sockaddr *)&client_addr, &socklen);
	if (clientFd == -1)
		throw std::runtime_error("Can't accept the connexion with the client");
	addToEpoll(clientFd, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
	client = new Client(clientFd, &server);
	server.addClientToMap(client);
}

Client *GlobalData::searchClient(const int fd)  {
	std::map<int, Server>::iterator it = _servMap.begin();
	std::map<int, Server>::iterator end = _servMap.end();

	while (it != end) {
		if (it->second.ifClientInServer(fd) == true) {
			break;
		}
		++it;
	}
	return it->second.getClient(fd);
}

Server *GlobalData::getServerWithClientFd(const int fd)  {
	std::map<int, Server>::iterator it = _servMap.begin();
	std::map<int, Server>::iterator end = _servMap.end();

	while (it is_not end) {
		if (it->second.ifClientInServer(fd) is true) {
			break;
		}
		++it;
	}
	return &it->second;
}

void GlobalData::handleClientIn(int fd) {
	Server *server = getServerWithClientFd(fd);

	try {
		server->addClientRequest(fd);
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void GlobalData::handleClientOut(int fd) {
	Server *server;
	std::ifstream file;

	server = getServerWithClientFd(fd);
	try {
		server->giveClientResponse(fd);
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		removeClient(fd);
	}
}


void GlobalData::removeClient(int fd) {
	Server *server = getServerWithClientFd(fd);

	server->removeClientInMap(fd);
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
}

bool GlobalData::isServerFd(const int &fd) {
	return _servMap.find(fd) is_not _servMap.end();
}

void	GlobalData::handleEvent( struct epoll_event& event ) {
	int	fd = event.data.fd;

	if (isServerFd(fd) is true) {
		try {
			addNewClient(_servMap[fd]);
		} catch (std::exception &e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	else if (event.events & (EPOLLRDHUP | EPOLLHUP))
		removeClient(fd);
	else if (event.events & EPOLLIN)
		handleClientIn(fd);
	else if (event.events & EPOLLOUT)
		handleClientOut(fd);
}

void GlobalData::runServers(std::vector<Server> &servVec) {
	int	fdsReady;

	initServers(servVec);
	while (g_running) {
		fdsReady = waitFdsToBeReady();
		for (int i = 0; i < fdsReady; i++) {
			handleEvent(_events[i]);
		}
	}
}

void GlobalData::closeServers(void) {
	
}
