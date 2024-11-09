#include "../../../includes/Client.hpp"
#include <unistd.h>
#include <iostream>

Client::Client() {
	this->_fd = -1;
	this->_readyToResponse = false;
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

void Client::setReadyToresponse(bool boolean) {
	this->_readyToResponse = boolean;
}

bool const &Client::isReadyToResponse(void) const {
	return	this->_readyToResponse;
} 


void Client::pushRequest(char str[BUFFER_SIZE]) {
	this->_request.addRequest(str);
	std::cout << str;
}