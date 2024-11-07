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
#include <iomanip>
#include <cstring>
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

bool g_running = true;
#include "../../includes/Parser.hpp"

Server::Server(void)  
	: _socket_fd(-1), _epoll_fd(-1) {
	
}

Server::~Server(void) {
	if (this->_socket_fd != -1)
		close(this->_socket_fd);
	  // delete [] this->_events;
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

Server &Server::operator=(Server const &other) {
	this->_socket_fd = other._socket_fd;
	this->_epoll_fd = other._epoll_fd;
	return *this;
}

void Server::init(void) {
	// Open socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd == -1) {
		throw ErrorException("Can't open socket");
	}
	// config address and port
	
	std::memset(&this->_server_addr, 0, sizeof(struct sockaddr_in));
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(PORT);

	// Link socket
	if (bind(this->_socket_fd, (struct sockaddr*)&this->_server_addr, sizeof(this->_server_addr)) < 0) {
        std::cerr << "Error when socket linking" << std::endl;
        throw ErrorException("Can't bind the socket");
	}

	if (listen(this->_socket_fd, MAX_CLIENTS) < 0) {
        std::cerr << "Error when listening" << std::endl;
        throw ErrorException("Can't listen on this socket");		
	}
}

int Server::waitFdsToBeReady(void) {
	return epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, -1);
}

void Server::addClientToMap(Client client) {
	this->_clientMap[client.getClientFd()] = client;
}

void Server::removeClient(int fd) {
	printf("[+] connection closed\n");
	epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);	
}

void Server::run(void) {
	this->_epoll_fd = epoll_create1(0); // TODO: Secure this
	getSocketFd(this->_epoll_fd, SET);
	this->addToEpoll(this->_socket_fd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLHUP);
	RawBits raw;
	
	std::cout << "listening on : http://127.0.0.1:" << PORT << std::endl;
	int nbFdsReady;
	while (g_running) {
		nbFdsReady = this->waitFdsToBeReady(); // TODO: Secure this (maybe) not sure
		for (int i = 0; i < nbFdsReady; i++) {
			if (this->_events[i].data.fd == this->_socket_fd) {
				this->addNewClient();
			}
			else if (this->_events[i].events & EPOLLIN) {
				handleClientIn(this->_events[i].data.fd);
			}
			else if (this->_events[i].events & EPOLLOUT) {
					const char *response = "HTTP/1.1 200 OK\r\n\r\nje suis en train de test!";
            	    send(this->_events[i].data.fd, response, strlen(response), MSG_EOR);
			}
			else {
				printf("[+] unexpected\n");
			}
			if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				this->removeClient(this->_events[i].data.fd);
				continue;
			}
		}
	}
}
