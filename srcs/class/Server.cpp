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
#include "../../includes/Parser.hpp"

Server::Server(void)  
	: _socket_fd(-1), _epoll_fd(-1) {
	this->_events = new struct epoll_event[MAX_EVENTS];
}

Server::~Server(void) {
	if (this->_socket_fd != -1)
		close(this->_socket_fd);
	  // delete [] this->_events;
}

void Server::addData(Data* data) {
  _data = data;
}

void ifSignal(int sig) {
	(void)sig;
	close(getSocketFd(0, GET));
	close(getEpollFd(0, GET));
	exit(EXIT_FAILURE);
}

void Server::signalHandle(void) {
	signal(SIGINT, ifSignal);
	signal(SIGQUIT, ifSignal);
	signal(SIGTSTP, ifSignal);
}

void Server::init(void) {
	// Open socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd == -1) {
		std::cerr << "Error when socket creation" << std::endl;
		exit(EXIT_FAILURE);
	}
	// config address and port
	
	getSocketFd(this->_socket_fd, SET);
	std::memset(&this->_server_addr, 0, sizeof(struct sockaddr_in));
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(PORT);

	// Link socket
	if (bind(this->_socket_fd, (struct sockaddr*)&this->_server_addr, sizeof(this->_server_addr)) < 0) {
        std::cerr << "Error when socket linking" << std::endl;
        close(this->_socket_fd);
        exit(EXIT_FAILURE);		
	}

	if (listen(this->_socket_fd, MAX_CLIENTS) < 0) {
        std::cerr << "Error when listening" << std::endl;
        close(this->_socket_fd);
        exit(EXIT_FAILURE);			
	}
}

void Server::addToEpoll(int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		std::cerr << "Epoll add" << std::endl;
		exit(1);
	}
}

int Server::waitFdsToBeReady(void) {
	return epoll_wait(this->_epoll_fd, this->_events, MAX_EVENTS, -1);
}

void Server::addNewClient(void) {
	int client_fd;
	struct sockaddr_in client_addr;
	unsigned int socklen;

	socklen = sizeof(client_addr);
	client_fd = accept(this->_socket_fd,  (struct sockaddr *)&client_addr, &socklen); // TODO: Secure this
	this->addToEpoll(client_fd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLHUP);
}

void Server::removeClient(int fd) {
	printf("[+] connection closed\n");
	epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);	
}

void handleClientIn(int fd) {
	unsigned char buf;
	int n;
	RawBits raw;

	std::cout << "start" << std::endl;
	while (true) {
		n = recv(fd, &buf, 1, MSG_DONTWAIT);
		// n = read(this->_events[i].data.fd, &buf, 1);
		if (n <= 0) {
			break;
		}
		else {
			// print_bytes(&buf, n);
			std::cout << buf;
			raw.pushBack(buf);
		}
	}
	std::cout << "finish" << std::endl;	
}

void handleClientOut(int fd) {
  const char *html_content = 
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>Page de test</title></head>"
        "<body><h1>Bienvenue sur mon serveur !</h1><p>Ceci est une page HTML.</p></body>"
        "</html>";
  std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    std::ostringstream oss;
    oss << strlen(html_content);
    response += "Content-Length: " + oss.str() + "\r\n";
    response += "\r\n"; // Séparateur entre en-têtes et contenu
    response += html_content; // Ajouter le contenu HTML

	send(fd, response.c_str(), response.size(), MSG_EOR);	
}

void Server::run(void) {
	this->_epoll_fd = epoll_create1(0); // TODO: Secure this
	getSocketFd(this->_epoll_fd, SET);
	this->addToEpoll(this->_socket_fd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLHUP);
	RawBits raw;
	
	std::cout << "listening on : http://127.0.0.1:" << PORT << std::endl;
	int nbFdsReady;
	while (true) {
		nbFdsReady = this->waitFdsToBeReady(); // TODO: Secure this (maybe) not sure
		for (int i = 0; i < nbFdsReady; i++) {
			if (this->_events[i].data.fd == this->_socket_fd) {
				this->addNewClient();
			}
			else if (this->_events[i].events & EPOLLIN) {
				handleClientIn(this->_events[i].data.fd);
			}
			else if (this->_events[i].events & EPOLLOUT) {
				handleClientOut(this->_events[i].data.fd);
			}
			if (this->_events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				this->removeClient(this->_events[i].data.fd);
				continue;
			}
		}
	}
}
