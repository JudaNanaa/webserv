#include "../../includes/GlobalData.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/RawBits.hpp"
#include "../../includes/includes.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <sys/epoll.h>
#include <vector>

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
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		throw std::runtime_error("Epoll add error");
	}
}

void GlobalData::initServers(std::vector<Server> &servVec) {
	std::vector<Server>::iterator it = servVec.begin();
	std::vector<Server>::iterator end = servVec.end();

	this->_epoll_fd = epoll_create1(0); // TODO: Secure this
	while (it != end) {
		it->init();
		this->addToEpoll(it->getSocketFd(), EPOLLIN); // TODO: try catch this
		this->_servMap[it->getSocketFd()] = *it;
		it++;
	}
}

int GlobalData::waitFdsToBeReady(void) {
	return epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, 100);
}

void GlobalData::addNewClient(Server &server) {
	Client *client;
	int clientFd;
	struct sockaddr_in client_addr;
	unsigned int socklen;

	client = new Client();
	socklen = sizeof(client_addr);
	clientFd = accept(server.getSocketFd(),  (struct sockaddr *)&client_addr, &socklen); // TODO: Secure this
	if (clientFd == -1)
		throw std::runtime_error("Can't accept the connexion with the client");
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Setting flags with fcntl failed");
	this->addToEpoll(clientFd, EPOLLIN | EPOLLOUT | EPOLLRDHUP); // TODO: try catch this

	client->setClientFd(clientFd);
	server.addClientToMap(*client);
}

Client &GlobalData::searchClient(const int fd)  {
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

void GlobalData::handleClientIn(int fd) {
	unsigned char buf;
	int n;
	Client client;
	RawBits raw;

	
	client = this->searchClient(fd);
	std::cout << "start" << std::endl;
	while (true) {
		n = recv(fd, &buf, 1, MSG_DONTWAIT);
		if (n <= 0) {
			break;
		}
		else {
			std::cout << buf;
			raw.pushBack(buf);
		}
	}
	std::cout << "finish" << std::endl;	
}

void GlobalData::handleClientOut(int fd) {
  const char *html_content = 
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>Page de test</title></head>"
        "<body><h1>Bienvenue sur mon serveur !</h1><p>Ceci est une page HTML.</p></body>"
        "</html>";
  std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    std::ostringstream oss;
    oss << strlen(html_content);
    response += "Content-Length: " + oss.str() + "\r\n";
    response += "\r\n"; // Séparateur entre en-têtes et contenu
    response += html_content; // Ajouter le contenu HTML

	send(fd, response.c_str(), response.size(), MSG_EOR);	
}

void GlobalData::removeClient(int fd) {
	std::map<int, Server>::iterator it = this->_servMap.begin();
	std::map<int, Server>::iterator end = this->_servMap.end();

	while (it != end) {
		if (it->second.ifClientInServer(fd) == true) {
			break;
		}
		++it;
	}
	it->second.removeClientInMap(fd);
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
			if (isServerFd(fd) == true && (this->_events[i].events & EPOLLIN)) {
				this->addNewClient(this->_servMap[fd]);
			}
			else if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				this->removeClient(fd);
			}
			else {
				if (this->_events[i].events & EPOLLIN) {
					this->handleClientIn(fd);
				}
				if (this->_events[i].events & EPOLLOUT) {
					this->handleClientOut(fd);
				}
			}
		}
	}
}

void GlobalData::closeServers(void) {

}