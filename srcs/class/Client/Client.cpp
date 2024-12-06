#include "Client.hpp"
#include <csignal>
#include <unistd.h>
#include <iostream>

Client::Client(int const &fd, Server *server)
	: _fd(fd), _server(server) {
	_request = new Request(this);
	_readyToResponse = false;
	_readyToParseHeader = false;
	_pid = -1;
	_CGIStatus = -1;
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

void Client::setResponse(const std::string &code)
{
	setReadyToresponse(true);
	_request->setResponsCode(code);
}

void Client::setCGIStatus(const int &status) {
	if (WIFEXITED(status)) {
		_CGIStatus = WEXITSTATUS(status);
	} else if (WIFSIGNALED(status)) {
		_CGIStatus = 128 + WTERMSIG(status);
	} else if (WIFSTOPPED(status)) {
		_CGIStatus = 128 + WSTOPSIG(status);
	}
	if (_CGIStatus == 1)
		_request->setResponsCode("500");
	else if (_CGIStatus == 128 + SIGALRM)
		_request->setResponsCode("504");
}

const int &Client::getCGIStatus(void) const {
	return _CGIStatus;
}


void Client::setPid(const int &pid) {
	_pid = pid;
}

const int &Client::getPid(void) const {
	return _pid;
}

void	Client::setCGIFD(const int &fd) {
	_pipeFD = fd;

}

const int &Client::getCGIFD(void) {
	return _pipeFD;
}

void	Client::setParentToCGI(const int &fd) {
	_ParentToCGI = fd;
}

const int &Client::getParentToCGI(void) {
	return _ParentToCGI;
}

const bool &Client::getReadyToParseHeader(void) const {
	return _readyToParseHeader;
}

const bool &Client::getReadyToParseBody(void) const {
	return _readyToParseBody;
}

void Client::setUseBuffer(const bool &boolean) {
	_useBuffer = boolean;
}

bool Client::getUseBuffer(void) const {
	return _useBuffer;
}


const t_state &Client::whatToDo(void) const {
	return _request->getStatus();
}

const int &Client::getClientFd(void) const {
	return _fd;
}

void Client::setReadyToresponse(const bool &boolean) {
	_readyToResponse = boolean;
}

bool const &Client::isReadyToResponse(void) const {
	return	_readyToResponse;
} 

Request *Client::getRequest(void) {
	return _request; 
}

void Client::pushHeaderRequest(char *str, const int &n) {
	t_parse result;

	result = _request->addHeaderRequest(str, n);
	if (result == READY_PARSE_HEADER)
		_readyToParseHeader = true;
}

void	Client::cleanRequest( void ) {
	if (_request != NULL)
		delete _request;
	_request = new Request(this);
}

void Client::afterResponse(void) {
	cleanRequest();
	setPid(-1);
	setCGIFD(-1);
	setReadyToresponse(false);
}
