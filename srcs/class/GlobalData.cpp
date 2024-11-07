#include "../../includes/GlobalData.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/RawBits.hpp"
#include "../../includes/includes.hpp"
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
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
		throw std::invalid_argument("Epoll add error");
	}
}

void GlobalData::initServers(std::vector<Server> &servVec) {
	std::vector<Server>::iterator it = servVec.begin();
	std::vector<Server>::iterator end = servVec.end();
	
	while (it != end) {
		it->init();
		it++;
	}
	this->initEpoll(servVec);
}

void GlobalData::initEpoll(std::vector<Server> &servVec) {
	this->_epoll_fd = epoll_create1(0); // TODO: Secure this
	std::vector<Server>::iterator it = servVec.begin();
	std::vector<Server>::iterator end = servVec.end();
	
	while (it != end) {
		addToEpoll(it->getSocketFd(), EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLHUP);
		this->_servMap[it->getSocketFd()] = *it;
		it++;
	}
}

int GlobalData::waitFdsToBeReady(void) {
	return epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, -1);
}

void GlobalData::addNewClient(Server &server) {
	Client client;
	int clientFd;
	struct sockaddr_in client_addr;
	unsigned int socklen;

	socklen = sizeof(client_addr);
	clientFd = accept(server.getSocketFd(),  (struct sockaddr *)&client_addr, &socklen); // TODO: Secure this
	if (clientFd == -1)
		throw std::invalid_argument("Can't accept the connexion with the client");
	std::cout << "client fd: " << clientFd << std::endl;
	this->addToEpoll(clientFd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLHUP);
	client.setClientFd(clientFd);
	server.addClientToMap(client);
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
		// n = read(this->_events[i].data.fd, &buf, 1);
		if (n <= 0) {
			break;
		}
		else {
			// print_bytes(&buf, n);
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

void GlobalData::runServers(std::vector<Server> &servVec) {
	this->initServers(servVec);
	int nbFdsReady;
	while (g_running) {
		nbFdsReady = this->waitFdsToBeReady(); // TODO: Secure this (maybe) not sure
		for (int i = 0; i < nbFdsReady; i++) {

			if (this->_servMap.find(this->_events[i].data.fd) != this->_servMap.end()) { // find socketFd
				// this->_servMap[this->_events[i].data.fd]->addNewClient();
				std::cout << "new user on fd " << this->_events[i].data.fd << std ::endl;
				this->addNewClient(this->_servMap[this->_events[i].data.fd]);
			}
			else if (this->_events[i].events & EPOLLIN) {
				std::cout << "New input" << std::endl;
				this->handleClientIn(this->_events[i].data.fd);
			}
			else if (this->_events[i].events & EPOLLOUT) {
				std::cout << "New output" << std::endl;
				this->handleClientOut(this->_events[i].data.fd);
			}
			if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				std::cout << "Remove user" << std::endl;
				this->removeClient(this->_events[i].data.fd);
				continue;
			}
		}
	}
}

void GlobalData::closeServers(void) {

}