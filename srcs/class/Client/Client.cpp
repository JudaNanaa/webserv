#include "Client.hpp"
#include <unistd.h>
#include <iostream>

Client::Client() {
	_request = NULL;
}

Client::Client(int const fd, Server *server)
	: _fd(fd), _server(server) {
	_request = new Request();
	this->_readyToResponse = false;
	_readyToParseHeader = false;
}

Client::~Client() {
	if (_request)
		delete _request;
	std::cerr << "client destroyed" << std::endl;
}

Client &Client::operator=(Client const &other) {
	if (this != &other) {
		this->_fd = other._fd;
		this->_readyToParseHeader = other._readyToParseHeader;
		this->_readyToResponse = other._readyToResponse;
		this->_request = other._request;
		this->_server = other._server;
	}
	return *this;
}

const bool &Client::getReadyToParseHeader(void) const {
	return _readyToParseHeader;
}

const bool &Client::getReadyToParseBody(void) const {
	return _readyToParseBody;
}

const t_state &Client::whatToDo(void) const {
	return _request->getStatus();
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

Request *Client::getRequest(void) {
	return _request; 
}

void Client::pushHeaderRequest(char *str, int n) {
	t_parse result;

	result = this->_request->addHeaderRequest(str, n);
	if (result == READY_PARSE_HEADER) {
		_readyToParseHeader = true;
	}
}

void Client::pushBodyRequest(char *str, int n) {
	t_parse result;

	result = this->_request->addBodyRequest(str, n);
	if (result == READY_PARSE_BODY) {
		_readyToParseBody = true;
	}
}

void Client::setServerReq(Server *server) {
  _request->addServer(server);
}

void	Client::cleanRequest( void ) {
	if (_request != NULL)
		delete _request;
	_request = new Request();
}
