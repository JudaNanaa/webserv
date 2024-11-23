#include "Client.hpp"
#include <unistd.h>
#include <iostream>

Client::Client(int const fd, Server *server)
	: _fd(fd), _server(server) {
	_request = new Request(this);
	_readyToResponse = false;
	_readyToParseHeader = false;
}

Client::~Client() {
	if (_request)
		delete _request;
	std::cerr << "client destroyed" << std::endl;
}

Client &Client::operator=(Client const &other) {
	if (this != &other) {
		_fd = other._fd;
		_readyToParseHeader = other._readyToParseHeader;
		_readyToResponse = other._readyToResponse;
		_request = other._request;
		_server = other._server;
	}
	return *this;
}

const bool &Client::getReadyToParseHeader(void) const {
	return _readyToParseHeader;
}

const bool &Client::getReadyToParseBody(void) const {
	return _readyToParseBody;
}

void Client::setUseBuffer(bool boolean) {
	_useBuffer = boolean;
}

bool Client::getUseBuffer(void) const {
	return _useBuffer;
}


const t_state &Client::whatToDo(void) const {
	return _request->getStatus();
}

int Client::getClientFd(void) const {
	return _fd;
}

void Client::setReadyToresponse(bool boolean) {
	_readyToResponse = boolean;
}

bool const &Client::isReadyToResponse(void) const {
	return	_readyToResponse;
} 

Request *Client::getRequest(void) {
	return _request; 
}

void Client::pushHeaderRequest(char *str, int n) {
	t_parse result;

	result = _request->addHeaderRequest(str, n);
	if (result == READY_PARSE_HEADER)
		_readyToParseHeader = true;
	else
		_readyToParseBody = false;
}

void Client::pushBodyRequest(char *str, int n) {
	_request->addBodyRequest(str, n, _useBuffer);
	// if (result == READY_PARSE_BODY)
	// 	_readyToParseBody = true;
	// else if (result == ERROR)
	// 	_readyToResponse = true;
	// else
	// 	_readyToParseBody = false;
}

void Client::setServerReq(Server *server) {
  _request->addServer(server);
}

void	Client::cleanRequest( void ) {
	if (_request != NULL)
	{
		_request->cleanFiles();
		delete _request;
	}
	_request = new Request(this);
}
