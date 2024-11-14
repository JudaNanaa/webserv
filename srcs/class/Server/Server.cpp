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
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fcntl.h>
#include <iomanip>
#include <cstring>
#include <iterator>
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
#include "../RawBits/RawBits.hpp"
#include "../Request/Request.hpp"
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
	int opt = 1;
	int keepAlive = 1;
	// Open socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_socket_fd == -1) {
		throw std::runtime_error("Can't open socket");
	}
	
	if(setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) { 
        throw std::runtime_error("Could not set socket options");	
    }
 
	if(setsockopt(this->_socket_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(keepAlive)) == -1) { 
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
	std::stringstream ss;
	ss << this->_data->_port;
	this->_host[0] = "127.0.0.1:" + ss.str();
	this->_host[1] = "localhost:" + ss.str();
	std::cout << "server on : http://" << this->_host[0] << std::endl;
}

void Server::addClientToMap(Client *client) {
	this->_clientMap[client->getClientFd()] = client;
}

bool Server::ifClientInServer(int fd) const {
	if (this->_clientMap.find(fd) == this->_clientMap.end()) {
		return false;
	}
	return true;
}

Client *Server::getClient(int fd) {
	return this->_clientMap[fd];
}

void Server::removeClientInMap(int fd) {
	this->_clientMap.erase(fd);
}

int Server::nbOfClient(void) const {
	return this->_clientMap.size();
}

bool Server::isServerHost(std::string const &str) const {
	if (_host[0] == str || _host[1] == str) {
		return true;
	}
	return false;
}

void	Server::_parseRequestLine( std::string line, Request *clientRequest) {

	if (line.find(": ") == std::string::npos)
		throw std::invalid_argument("invalid line: " + line);

	std::string	value = line.substr(line.find(": ") + 2);

	std::string key = line.erase(line.find(": "));	// erase the value (keep the key)
	clientRequest->addRequestToMap(key, value);
}

void Server::_parseClientHeader(Client *client) {
	Request *clientRequest;
	std::string header;
	std::vector<std::string> headerSplit;
	std::vector<std::string> lineSplit;

	clientRequest = client->getRequest();
	header = clientRequest->getRawRequest()->getHeader();
	headerSplit = split(header, "\r\n");

	std::cout << "DEBUG HEADER: \n" << header << std::endl;
	if (std::count(headerSplit[0].begin(), headerSplit[0].end(), ' ') != 2) {
		// La premiere ligne est pas bonne donc faire une reponse en fonction
		throw std::invalid_argument("Error header: " + headerSplit[0]);
	}

	lineSplit = split(headerSplit[0], " ");
	if (lineSplit.size() != 3) { // not always 3 part
		// La premiere ligne est pas bonne donc faire une reponse en fonction
		throw std::invalid_argument("Error header: " + headerSplit[0]);
	}

	checkAllowMethodes(lineSplit[0]);

	clientRequest->path(lineSplit[1]);
  std::cout << "-------------------------------------PATH : " + clientRequest->path() << std::endl;

	if (lineSplit[2].compare("HTTP/1.1") != 0) {
		// le htpp nest pas bon !!
		throw std::invalid_argument("Error header: " + headerSplit[2]);
	}

	// lineSplit = split(headerSplit[1], ": "); // check line host
	// if (lineSplit.size() != 2) {
	// 	throw std::invalid_argument("Error header: " + headerSplit[1]);
	// }
	/*if (lineSplit[0] != "Host") {*/
	/*	throw std::invalid_argument("Error header: " + headerSplit[1]);*/
	/*}*/
	/*clientRequest->host(lineSplit[1]);*/
	/*if (isServerHost(clientRequest->host()) == false) { // check si le host est bien celui du server*/
	/*	throw std::invalid_argument("Error header: Not the server host: " + lineSplit[1]);*/
	/*}*/

	for (std::vector<std::string>::const_iterator it = headerSplit.begin() + 1, ite = headerSplit.end();
			it != ite; it++) {
		_parseRequestLine(*it, clientRequest);
	}

	// std::cout << "REQUEST:\n" << *clientRequest << std::endl;

	if (clientRequest->isKeyfindInHeader("Content-Length") == true) {
		clientRequest->setSizeBody(atoi(clientRequest->find("Content-Length").c_str()));
	} else {
		client->setReadyToresponse(true);
	}
}

void Server::_parseClientBody(Client *client) {
	const char *clientBody;

	clientBody = client->getRequest()->getRawRequest()->getBody();
	// std::cout << clientBody->getContent() << std::endl;
	client->setReadyToresponse(true);
}

void Server::addClientRequest(int fd) {
	char buff[BUFFER_SIZE + 1];
	int n;
	Client *client;

	client = getClient(fd);
	n = recv(fd, buff, BUFFER_SIZE, MSG_DONTWAIT);
	if (n == -1) {
		throw std::runtime_error("Can't recv the message !");
	}
	if (n < 0)
		return;
	buff[n] = '\0';
	if (client->whatToDo() == ON_HEADER) {
	 	client->pushHeaderRequest(&buff[0], n);
	 	if (client->getReadyToParseHeader()) {
	 		_parseClientHeader(client);
	  }
	}
	else if (client->whatToDo() == ON_BODY) {
		client->pushBodyRequest(buff, n);
	 	if (client->getReadyToParseBody()) {
	 		_parseClientBody(client); //Parse body
	 	}
	}
}

bool Server::checkAllowMethodes(std::string methode) {
  if (methode.compare("GET") == 0) {
	return (_data->_allowedMethods & GET_);
  }
  if (methode.compare("POST")  == 0) {
	return (_data->_allowedMethods & POST_);
  }
  if (methode.compare("DELETE")  == 0) {
	return (_data->_allowedMethods & DELETE_);
  }
  if (methode.compare("OPTIONS")  == 0) {
	return (_data->_allowedMethods & OPTIONS_);
  }
  if (!methode.empty())
    throw std::invalid_argument("Invalid methode : " + methode);
  return false;
}
