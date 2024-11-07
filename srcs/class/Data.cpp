#include "../../includes/Data.hpp"
#include <cstdlib>
#include <vector>

Data::Data() { 
	this->_epoll_fd = -1;
}

Data::~Data() { 
	if (this->_epoll_fd != -1)
		close(this->_epoll_fd);
}


void Data::addToEpoll(int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr << "Epoll add" << std::endl;
		exit(1);
	}
}

void Data::initServers(void) {
	std::vector<Server>::iterator it = this->servVec.begin();
	std::vector<Server>::iterator end = this->servVec.end();
	
	while (it != end) {
		it->init();
		it++;
	}
}

int Data::initEpoll() {
	this->_epoll_fd = epoll_create1(0); // TODO: Secure this
	getSocketFd(this->_epoll_fd, SET);
	std::vector<Server>::iterator it = this->servVec.begin();
	std::vector<Server>::iterator end = this->servVec.end();
	
	while (it != end) {
		it->init();
		it++;
	}

	return EXIT_SUCCESS;
}

void Data::runServers(void) {
	this->initServers();
}

void Data::closeServers(void) {

}