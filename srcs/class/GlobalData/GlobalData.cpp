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
	this->_epoll_fd = -1;
}

GlobalData::~GlobalData() { 
	if (this->_epoll_fd != -1)
		close(this->_epoll_fd);
}


void GlobalData::addToEpoll(int fd, uint32_t events)
{
	struct epoll_event ev;

	ev.events = events;
	ev.data.fd = fd;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Setting flags with fcntl failed");
	}
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		throw std::runtime_error("Epoll add error");
	}
}

void GlobalData::initServers(std::vector<Server> &servVec) {
	std::vector<Server>::iterator servIt = servVec.begin();
	std::vector<Server>::iterator end = servVec.end();

	this->_epoll_fd = epoll_create1(0); // TODO: Secure this
	while (servIt != end) {
		servIt->init();
		addToEpoll(servIt->getSocketFd(), EPOLLIN); // TODO: try catch this
		_servMap[servIt->getSocketFd()] = *servIt;
		servIt++;
	}
}

int GlobalData::waitFdsToBeReady(void) {
	return epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, -1);
}

void GlobalData::addNewClient(Server &server) {
	Client *client;
	int clientFd;
	struct sockaddr_in client_addr;
	unsigned int socklen;

	socklen = sizeof(client_addr);
	clientFd = accept(server.getSocketFd(),  (struct sockaddr *)&client_addr, &socklen); // TODO: Secure this
	if (clientFd == -1)
		throw std::runtime_error("Can't accept the connexion with the client");
	this->addToEpoll(clientFd, EPOLLIN | EPOLLOUT | EPOLLRDHUP); // TODO: try catch this

	client = new Client(clientFd, &server);
	client->setServerReq(&server);
	server.addClientToMap(client);
}

Client *GlobalData::searchClient(const int fd)  {
	std::map<int, Server>::iterator it = this->_servMap.begin();
	std::map<int, Server>::iterator end = this->_servMap.end();

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

	while (it != end) {
		if (it->second.ifClientInServer(fd) == true) {
			break;
		}
		++it;
	}
	return &it->second;
}

void GlobalData::handleClientIn(int fd) {
	Server *server;

	server = getServerWithClientFd(fd);
	try {
		server->addClientRequest(fd);
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		removeClient(fd);
	}
}

void GlobalData::handleClientOut(int fd) {
	std::ifstream file;

	// file.open("URIs/original.html");
	Client *client = searchClient(fd);
	if (client->isReadyToResponse() == false)
		return;
	if (client->getRequest()->path() == "/") {
		file.open((client->_server->_data->_root + client->_server->_data->_index).c_str());
		std::cout << "if root : " << client->_server->_data->_root + client->_server->_data->_index << std::endl;

	}
	else {

		std::cout << "if no root : " << client->_server->_data->_root + client->getRequest()->path() << std::endl;
		file.open((client->_server->_data->_root + client->getRequest()->path()).c_str());
	}
	// std::cout << "PATH + '" << client->getRequest()->path() << "'" << std::endl;
	// std::cout << "debug : " << client._server->_data->_root + client._server->_data->_index << std::endl;
	// file.open(server.data.root + server.data.index) <---- TODO: C'est ca qu'on dois faire si index est pas trouvé et que auto index = on on doit renvoyer la liste des fichier
	if (file.fail()) {
		throw std::runtime_error("Can't open the file");
	}


  /*std::cout << "SEND RESPONSE" << std::endl;*/
  std::ostringstream buffer;
  buffer << file.rdbuf();
  std::string html_content = buffer.str();

	std::ostringstream oss;
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
	oss << html_content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
    response += "\r\n";

    response += html_content;

	send(fd, response.c_str(), response.size(), MSG_EOR);
	client->cleanRequest();
}

void GlobalData::removeClient(int fd) {
	Server *server;

	server = getServerWithClientFd(fd);
	server->removeClientInMap(fd);
	epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	printf("[+] connection closed\n");
}

bool GlobalData::isServerFd(const int &fd) const {
	return this->_servMap.find(fd) != this->_servMap.end();
}

void GlobalData::runServers(std::vector<Server> &servVec) {
	int nbFdsReady;
	int fd;

	this->initServers(servVec);
	while (g_running) {
		nbFdsReady = this->waitFdsToBeReady(); // TODO: Secure this (maybe) not sure
		for (int i = 0; i < nbFdsReady; i++) {
			fd = this->_events[i].data.fd;
			if (isServerFd(fd) == true) {
				std::cout << "\n--------------------ADDING CLIENT--------------------\n" << std::endl;
				this->addNewClient(this->_servMap[fd]);
			}
			else if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				std::cout << "\n--------------------REMOVING CLIENT--------------------\n" << std::endl;
				this->removeClient(fd);
			}
			else {
				if (this->_events[i].events & EPOLLIN) {
				std::cout << "\n--------------------NEW REQUEST--------------------\n" << std::endl;
					this->handleClientIn(fd);
				}
				else if (this->_events[i].events & EPOLLOUT) {
					this->handleClientOut(fd);
				}
			}
		}
	}
}

void GlobalData::closeServers(void) {

}
