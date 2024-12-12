/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:16:36 by madamou           #+#    #+#             */
/*   Updated: 2024/11/07 16:12:18 by itahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm-generic/socket.h>
#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <iomanip>
#include <cstring>
#include <map>
#include <ostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
#include "Server.hpp"

Server::Server(void) {
	_socket_fd = -1;
}

Server::~Server(void) {
	if (this->_socket_fd is_not -1)
		close(this->_socket_fd);
}

void Server::freeAll(void) {
	delete _data;
}

void Server::init(void) {
	struct sockaddr_in server_addr;
	int opt = 1;
	// Open socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_socket_fd is -1)
		throw std::runtime_error("Can't open socket");
	
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) is -1)
        throw std::runtime_error("Could not set socket options");
 
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) is -1)
        throw std::runtime_error("Could not set socket options");
	// config address and port

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(this->_data->_port);

	// Link socket
	if (bind(this->_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Can't bind the socket");

	if (listen(this->_socket_fd, INT_MAX) < 0)
        throw std::runtime_error("Can't listen on this socket");
	std::stringstream ss;
	ss << this->_data->_port;
	this->_host[0] = "127.0.0.1:" + ss.str();
	this->_host[1] = "localhost:" + ss.str();
	std::cout << "server on : http://" << this->_host[0] << std::endl;
}

void Server::_addingBody(Client *client, const char *buff, const int &n)
{
	Request *clientRequest = client->getRequest();

	if (clientRequest->getRequestType() is CGI)
		_writeBodyToCgi(client, buff, n);
	else
		clientRequest->addBodyRequest(buff, n, client->getUseBuffer());	
}

void Server::_handleDelete(Client* client) {
	Request	*request = client->getRequest();

	if (request->path().find("..") is_found)
		return (client->setResponse("403"));
	if (std::strncmp(request->path().c_str(), "/uploads/", 9) is_not 0)
		return (client->setResponse("403"));
	if (request->path() is "/uploads/post.html")
		return (client->setResponse("403"));
	if (request->path() is "/uploads/" || request->path() is "/uploads")
		return (client->setResponse("403"));
	

	if (access((_data->_root + request->path()).c_str(), F_OK) is_not 0) {
		client->setResponse("404");
		return ;
	}

	if (unlink((_data->_root + request->path()).c_str()) is -1)
		std::cerr << "DELETE: unlink() failed" << std::endl;
	client->setResponse("200");
}

t_state Server::addClientRequest(int fd) {
	char buff[BUFFER_SIZE];
	int n;
	Client *client = _getClient(fd);
	Request *clientRequest = client->getRequest();

	client->setUseBuffer(true);
	n = recv(fd, buff, BUFFER_SIZE, MSG_DONTWAIT);
	if (n is -1) {
		client->setResponse("505");
		throw std::runtime_error("Can't recv the message !");
	}
	else if (n is 0)
	{
		client->setResponse("400");
		throw std::runtime_error("Empty recv !");
	}
	if (client->whatToDo() is ON_HEADER)
		_addingHeader(client, buff, n);
	if (client->whatToDo() is ON_BODY)
		_addingBody(client, buff, n);
	if (clientRequest->method() is DELETE_ && clientRequest->getResponsCode() is "200")
		_handleDelete(client);
	return clientRequest->getState();
}
