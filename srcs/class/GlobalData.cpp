#include "../../includes/GlobalData.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/RawBits.hpp"
#include <cstdlib>
#include <exception>
#include <stdexcept>
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
		throw ErrorException("Epoll add error");
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
	getSocketFd(this->_epoll_fd, SET);
	std::vector<Server>::iterator it = servVec.begin();
	std::vector<Server>::iterator end = servVec.end();
	
	while (it != end) {
		addToEpoll(it->getSocketFd(), EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLHUP);
		this->_servMap[it->getSocketFd()] = *it;
		it++;
	}
}

int GlobalData::waitFdsToBeReady(void) {
	return epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, 2000);
}

void GlobalData::addNewClient(Server &server) {
	Client client;
	int clientFd;
	struct sockaddr_in client_addr;
	unsigned int socklen;

	socklen = sizeof(client_addr);
	clientFd = accept(server.getSocketFd(),  (struct sockaddr *)&client_addr, &socklen); // TODO: Secure this
	if (clientFd == -1)
		throw ErrorException("Can't accept the connexion with the client");
	this->addToEpoll(clientFd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLHUP);
	client.setClientFd(clientFd);
	server.addClientToMap(client);
}

Client &GlobalData::searchClient(const int fd) const {
	std::map<int, Server>::const_iterator it = this->_servMap.begin();
	std::map<int, Server>::const_iterator end = this->_servMap.end();

	while (it != end) {
		if ()
		++it;
	}
	return ;
}

void GlobalData::handleClientIn(int fd) {
	unsigned char buf;
	int n;
	RawBits raw;

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

void GlobalData::runServers(std::vector<Server> &servVec) {
	this->initServers(servVec);
	int nbFdsReady;
	while (g_running) {
		nbFdsReady = this->waitFdsToBeReady(); // TODO: Secure this (maybe) not sure
		for (int i = 0; i < nbFdsReady; i++) {
			if (this->_servMap.find(this->_events[i].data.fd) != this->_servMap.end()) { // find socketFd
				// this->_servMap[this->_events[i].data.fd]->addNewClient();
				this->addNewClient(this->_servMap[this->_events[i].data.fd]);
			}
			else if (this->_events[i].events & EPOLLIN) {
				this->handleClientIn(this->_events[i].data.fd);
			}
			else if (this->_events[i].events & EPOLLOUT) {
					const char *response = "HTTP/1.1 200 OK\r\n\r\nje suis en train de test!";
            	    send(this->_events[i].data.fd, response, strlen(response), MSG_EOR);
			}
			else {
				printf("[+] unexpected\n");
			}
			if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				this->removeClient(this->_events[i].data.fd);
				continue;
			}
		}
	}
}

void GlobalData::closeServers(void) {

}