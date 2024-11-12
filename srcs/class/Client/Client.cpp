#include "Client.hpp"
#include <unistd.h>
#include <iostream>

Client::Client() {

}

Client::Client(int const fd, Server *server)
	: _fd(fd), _server(server) {
	this->_readyToResponse = false;
	_readyToParseHeader = false;
}

Client::~Client() {
}

Client &Client::operator=(Client const &other) {
	this->_fd = other._fd;
	this->_readyToParseHeader = other._readyToParseHeader;
	this->_readyToResponse = other._readyToResponse;
	this->_request = other._request;
	this->_server = other._server;
	return *this;
}

const bool &Client::getReadyToParseHeader(void) const {
	return _readyToParseHeader;
}

const bool &Client::getReadyToParseBody(void) const {
	return _readyToParseBody;
}

const t_state &Client::whatToDo(void) const {
	return _request.getStatus();
}

int Client::getClientFd(void) const {
	return this->_fd;
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
	t_parse result;
	result = this->_request.addRequest(str);
	if (result == READY_PARSE_HEADER) {
		_readyToParseHeader = true;
	}
	else if (result == READY_PARSE_BODY) {
		_readyToParseBody = true;
	}
}

void Client::setServerReq(Server *server) {
  _request.addServer(server);
}
