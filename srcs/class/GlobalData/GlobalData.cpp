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

GlobalData::GlobalData()
	: _epoll_fd(-1) { 
}

GlobalData::~GlobalData() { 
	if (_epoll_fd != -1)
		close(_epoll_fd);
}

void GlobalData::_addToEpoll(const int &fd, const uint32_t &events)
{
	struct epoll_event ev;

	ev.events = events;
	ev.data.fd = fd;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) is -1)
		throw std::runtime_error("Setting flags with fcntl failed");
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &ev) is -1)
		throw std::runtime_error("Epoll add error");
}

void GlobalData::_initServers(std::vector<Server> &servVec) {
	std::vector<Server>::iterator servIt = servVec.begin();
	std::vector<Server>::iterator end = servVec.end();

	_epoll_fd = epoll_create1(0);
	if (_epoll_fd is -1)
		throw std::runtime_error("epoll_create1 failed !");
	while (servIt != end) {
		servIt->init();
		int servFd = servIt->getSocketFd();
		_addToEpoll(servFd, EPOLLIN);
		_servMap[servFd] = *servIt;
		servIt++;
	}
}

int GlobalData::_waitFdsToBeReady(void) {
	return epoll_wait(_epoll_fd, _events, MAX_EVENTS, -1);
}

void GlobalData::_addNewClient(Server &server) {
	Client *client;
	int clientFd;
	struct sockaddr_in client_addr;
	unsigned int socklen = sizeof(client_addr);

	try {
		clientFd = accept(server.getSocketFd(),  (struct sockaddr *)&client_addr, &socklen);
		if (clientFd is -1)
			throw std::runtime_error("Can't accept the connexion with the client");
		_addToEpoll(clientFd, EPOLLIN | EPOLLRDHUP | EPOLLHUP);
		client = new Client(clientFd, &server);
		server.addClientToMap(client);
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

Server *GlobalData::_getServerWithClientFd(const int &fd) {
	std::map<int, Server>::iterator it = _servMap.begin();
	std::map<int, Server>::iterator end = _servMap.end();

	while (it is_not end) {
		if (it->second.ifClientInServer(fd) is true)
			break;
		++it;
	}
	return &it->second;
}

void GlobalData::_modifyClientEvent(const int &fd, const uint32_t &events)
{
	struct epoll_event ev;

	ev.events = events;
	ev.data.fd = fd;	
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, fd, &ev) is -1)
		throw std::runtime_error("Epoll mod error");
}

void GlobalData::_handleClientIn(const int &fd) {
	Server *server;

	server = _getServerWithClientFd(fd);
	try {
		if (server->addClientRequest(fd) == RESPONSE)
			_modifyClientEvent(fd, EPOLLOUT | EPOLLRDHUP | EPOLLHUP); // Set event for epoll so now we wait for OUT events
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void GlobalData::_handleClientOut(const int &fd) {
	Server *server;
	std::ifstream file;

	server = _getServerWithClientFd(fd);
	try {
		if (server->giveClientResponse(fd) == SEND)
			_modifyClientEvent(fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP); // restore event for epoll so now we wait for IN events
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		_removeClient(fd);
	}
}

void GlobalData::_removeClient(const int &fd) {
	Server *server;

	server = _getServerWithClientFd(fd);
	server->removeClientInMap(fd);
	epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
}

bool GlobalData::_isServerFd(const int &fd) {
	return _servMap.find(fd) is_not _servMap.end();
}

void	GlobalData::_handleEvent(const struct epoll_event& event) {
	int	fd = event.data.fd;

	if (_isServerFd(fd) is true)
		_addNewClient(_servMap[fd]);
	else if (event.events & (EPOLLRDHUP | EPOLLHUP))
		_removeClient(fd);
	else if (event.events & EPOLLIN)
		_handleClientIn(fd);
	else if (event.events & EPOLLOUT)
		_handleClientOut(fd);
}

void GlobalData::runServers(std::vector<Server> &servVec) {
	int	fdsReady;

	_initServers(servVec);
	while (g_running) {
		fdsReady = _waitFdsToBeReady();
		for (int i = 0; i < fdsReady; i++)
			_handleEvent(_events[i]);
	}
}

void GlobalData::closeServers(void) {
	std::map<int, Server>::iterator it = _servMap.begin();
	std::map<int, Server>::iterator end = _servMap.end();

	while (it is_not end) {
		it->second.freeAll();
		++it;
	}
}
