#include "Client.hpp"
#include <csignal>
#include <unistd.h>
#include <iostream>

Client::Client(int const &fd, Server *server)
    :	_fd(fd),
		_pipeFD(-1),
    	_pid(-1),
    	_CGIStatus(-1),
    	_request(new Request(this, server)),
    	_server(server)
{
}

Client::~Client() {
	if (_request)
		delete _request;
	if (_pipeFD != -1)
		close(_pipeFD);
	close(_fd);
	std::cerr << "client destroyed" << std::endl;
}

void Client::setResponse(const std::string &code)
{
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
	if (_CGIStatus == 0)
		_request->setResponsCode("200");
	else if (_CGIStatus == 128 + SIGALRM)
		_request->setResponsCode("504");
	else
		_request->setResponsCode("500");
	if (_CGIStatus != 0)
		_request->setResponseCgi(false);
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

void Client::setUseBuffer(const bool &boolean) {
	_useBuffer = boolean;
}

bool Client::getUseBuffer(void) const {
	return _useBuffer;
}


const t_state &Client::whatToDo(void) const {
	return _request->getState();
}

const int &Client::getClientFd(void) const {
	return _fd;
}

Request *Client::getRequest(void) {
	return _request;
}

void Client::pushHeaderRequest(const char *str, const int &n) {
	_request->addHeaderRequest(str, n);
}

void	Client::cleanRequest( void ) {
	if (_request != NULL)
		delete _request;
	_request = new Request(this, _server);
}

void Client::afterResponse(void) {
	cleanRequest();
	setPid(-1);
	setCGIFD(-1);
}
