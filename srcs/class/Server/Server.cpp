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
#include "Server.hpp"
#include "../../../includes/utils.hpp"
#include "../../../includes/RawBits.hpp"
#include "../GlobalData/GlobalData.hpp"
#include "../Parser/Parser.hpp"

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
	struct sockaddr_in server_addr;
	const int opt = 1;
	// Open socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd == -1) {
		throw std::runtime_error("Can't open socket");
	}
	
	if(setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1) { 
        throw std::runtime_error("Could not set socket options");	
    } 
	// config address and port
	
	std::memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->_data->_port);

	// Link socket
	if (bind(this->_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Can't bind the socket");
	}

	if (listen(this->_socket_fd, MAX_CLIENTS) < 0) {
        throw std::runtime_error("Can't listen on this socket");		
	}
}

void Server::addClientToMap(Client &client) {
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
}

int Server::nbOfClient(void) const {
	return this->_clientMap.size();
}

bool Server::checkAllowMethodes(std::string methode) {
  bool result = true;
  if (methode.find(";") != std::string::npos)
    methode.erase(methode.find(';'),1);

  if (methode.find("GET") != std::string::npos) {
    _data->_allowedMethods & GET_ ? result = true : result = false;
    methode.erase(methode.find("GET"), 3);
  }
  if (result && methode.find("POST") != std::string::npos) {
    _data->_allowedMethods & POST_ ? result = true : result = false;
    methode.erase(methode.find("GET"), 3);
  }
  if (result && methode.find("DELETE") != std::string::npos) {
    _data->_allowedMethods & DELETE_ ? result = true : result = false;
    methode.erase(methode.find("GET"), 3);
  }
  if (result && methode.find("OPTIONS") != std::string::npos) {
    _data->_allowedMethods & OPTIONS_ ? result = true : result = false;
    methode.erase(methode.find("GET"), 3);
  }
  if (!methode.empty())
    throw std::invalid_argument("Invalid methode : " + methode);
  return result;
}
