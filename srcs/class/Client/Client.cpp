#include "Client.hpp"
#include <unistd.h>
#include <iostream>

Client::Client() {
	this->_fd = -1;
	this->_readyToResponse = false;
	_readyToParseHeader = false;
	std::cerr << "client creation" << std::endl;
}

Client::~Client() {
	// if (this->_fd != -1)
	// 	close(this->_fd);
	std::cerr << "client destructor" << std::endl;
}

bool Client::getReadyToParseHeader(void) const {
	return _readyToParseHeader;
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

Request &Client::getRequest(void) {
	return _request; 
}

void Client::pushRequest(char str[BUFFER_SIZE]) {
	if (this->_request.addRequest(str) == READY_PARSE_HEADER) {
		_readyToParseHeader = true;
	}
}

void Client::setServer(Server *server) {
	this->_server = server;
}

void Client::setServerReq(Server *server) {
  _request.addServer(server);
}
