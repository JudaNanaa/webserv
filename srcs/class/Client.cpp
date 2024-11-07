#include "../../includes/Client.hpp"
#include <unistd.h>

Client::Client() {
	this->_fd = -1;
}

Client::~Client() {
	if (this->_fd != -1)
		close(this->_fd);
}

int Client::getClientFd(void) const {
	return this->_fd;
}

void Client::setClientFd(int fd) {
	this->_fd = fd;
}
