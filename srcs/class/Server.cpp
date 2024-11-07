/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:16:36 by madamou           #+#    #+#             */
/*   Updated: 2024/11/07 16:00:23 by itahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <cstring>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>
#include "../../includes/Server.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/RawBits.hpp"
#include "../../includes/GlobalData.hpp"

bool g_running = true;
#include "../../includes/Parser.hpp"

Server::Server(void)  
	: _socket_fd(-1) {
	
}

Server::~Server(void) {
	if (this->_socket_fd != -1)
		close(this->_socket_fd);
}

void Server::addData(Data* data) {
  _data = data;
}

int Server::getSocketFd(void) const {
	return this->_socket_fd;
}

void ifSignal(int sig) {
	(void)sig;
	g_running = false;
}

void signalHandle(void) {
	signal(SIGINT, ifSignal);
	signal(SIGQUIT, ifSignal);
	signal(SIGTSTP, ifSignal);
}

void Server::init(void) {
	// Open socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd == -1) {
		throw std::invalid_argument("Can't open socket");
	}
	// config address and port
	
	std::memset(&this->_server_addr, 0, sizeof(struct sockaddr_in));
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(this->_data->_port);

	// Link socket
	if (bind(this->_socket_fd, (struct sockaddr*)&this->_server_addr, sizeof(this->_server_addr)) < 0) {
        std::cerr << "Error when socket linking" << std::endl;
        throw std::invalid_argument("Can't bind the socket");
	}

	if (listen(this->_socket_fd, MAX_CLIENTS) < 0) {
        std::cerr << "Error when listening" << std::endl;
        throw std::invalid_argument("Can't listen on this socket");		
	}
}

void Server::addClientToMap(Client client) {
	this->_clientMap[client.getClientFd()] = client;
}

bool Server::ifClientInServer(int fd) const {
	if (this->_clientMap.find(fd) == this->_clientMap.end()) {
		return false;
	}
	return true;
}

Client &Server::getClient(int fd) {
	return this->_clientMap[fd];
}

void Server::removeClientInMap(int fd) {

	this->_clientMap.erase(fd);
	printf("[+] connection closed\n");
}
