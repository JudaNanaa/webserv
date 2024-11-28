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
#include <cstdio>
#include <cstdlib>
#include <ctime>
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
#include <sys/wait.h>
#include <vector>
#include "Server.hpp"

Server::Server(void) {
	_socket_fd = -1;
}

Server::~Server(void) {
	if (this->_socket_fd != -1)
		close(this->_socket_fd);
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
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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

void	Server::handleLocation(Client *client) {
  Request* request = client->getRequest();
  Location* location = _data->checkLocation(request->path());
  //check des methodes

  if (!location->redirect().empty()) {
	request->setRedirect(true);
  }

  if ((location->allowedMethods() & request->method()) == 0) {
    request->setResponsCode("405");
    client->setReadyToresponse(true);
    return ;
  }
  //check de la body Size
	if (location->maxBodySize() > 0) {
		if ((unsigned)location->maxBodySize() < request->getContentLenght()) {

	    	request->setResponsCode("413");
    		client->setReadyToresponse(true);
    		return ;
		}
	}
}

bool	Server::isCgi( const std::string& path ) {
	/*		Ou trouver les cgi ?	*/
	// TODO
	std::size_t	extension;

	extension = path.find_last_of('.');
	if (extension == std::string::npos) {
		return (false);
	}
	if (_data->_cgi.find(path.substr(extension)) != _data->_cgi.end()) {	//	valid CGI
		return (true);
	} else {
		return (false);
	}
}

void	Server::handleCgi( Client *client ) {
	client->getRequest()->setIsACgi(true);
	CgiDefaultGesture(client);
}

void	Server::handleRequest( Client *client ) {
	Request *request = client->getRequest();

	if ((_data->_allowedMethods & request->method()) == 0) {
		request->setResponsCode("405");
		client->setReadyToresponse(true);
		return ;
	}
	//check de la body Size
	if (_data->_clientMaxBodySize >= 0) {
		if (_data->_clientMaxBodySize < request->getContentLenght()) {
			request->setResponsCode("413");
			client->setReadyToresponse(true);
			return ;
		}
	}
	if (request->getContentLenght() == -1) {
		client->setReadyToresponse(true);
	}
	// if (request->getMethode() & GET_ && request->getLenTotalBody() == request->getContentLenght())
	// 	client->setReadyToresponse(true);
}

void	Server::chooseParsing( Client *client ) {
	Request	*request = client->getRequest();

	if (_data->checkLocation(request->path()) != NULL) {
		std::cerr << "LOCATION" << std::endl;
		handleLocation(client);
	} else if (isCgi(request->path()) == true) {
		std::cerr << "CGI" << std::endl;
		handleCgi(client);
	} else {
		std::cerr << "DEFAULT" << std::endl;
		handleRequest(client);
	}
	request->setStatus(ON_BODY);
}

void Server::parseHeaderWithLocation(Client *client, Request *request) {
  (void)client;
  (void)request;
  
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
	header = clientRequest->getHeader();
	headerSplit = split(header, "\r\n");

	if (std::count(headerSplit[0].begin(), headerSplit[0].end(), ' ') != 2) {
		// La premiere ligne est pas bonne donc faire une reponse en fonction
		throw std::invalid_argument("Error header 1: " + headerSplit[0]);
	}
	lineSplit = split(headerSplit[0], " ");
	if (lineSplit.size() != 3) { // not always 3 part
		// La premiere ligne est pas bonne donc faire une reponse en fonction
		throw std::invalid_argument("Error header 2: " + headerSplit[0]);
	}
	clientRequest->setMethode(lineSplit[0]);
	clientRequest->path(lineSplit[1]);
	
	std::cout << "-------------------------------------PATH : " + clientRequest->path() << std::endl;
	if (lineSplit[2].compare(0, 6, "HTTP/1") != 0) {
		// le htpp nest pas bon !!
		clientRequest->setResponsCode("505");
		return;
	}
	for (std::vector<std::string>::const_iterator it = headerSplit.begin() + 1, ite = headerSplit.end();
			it != ite; it++) {
		_parseRequestLine(*it, clientRequest);
	}
	std::cerr << "REQUEST:\n" << *clientRequest << std::endl;
	if (clientRequest->isKeyfindInHeader("Content-Length") == true) {
		clientRequest->setSizeBody(std::atoll(clientRequest->find("Content-Length").c_str()));
		std::string bondary;
		//TODO : secure this (si Content-Lenght ou Content-Type ne sont pas present dans la requete on throw une exception et on ne renvoie pas de reponse au client)
    //alors qu'on doit en renvoyer une
		if (clientRequest->isKeyfindInHeader("Content-Type")) {
			if (clientRequest->find("Content-Type").find("boundary") != std::string::npos) {
				bondary = clientRequest->find("Content-Type").substr(clientRequest->find("Content-Type").find("boundary") + 9);
				clientRequest->setBondary(const_cast<char*>(bondary.c_str()));
			}
		}
	}
  	chooseParsing(client); // apre avoir recuperer les infos, on choisie le parsing approprier grace aux informations recuperer
}

void Server::checkCgi( void ) {
	std::map<int, Client*>::iterator it, ite;

	it = _clientMap.begin();
	ite = _clientMap.end();

	for (;it != ite; it++) {
		Client	*client = it->second;
		int		pid = client->getPid();

		if (pid == -1) {	// no 
			continue ;
		} else {
			int	status;
			switch (waitpid(pid, &status, WNOHANG)) {
				case -1: { std::cerr << "waitpid failed" << std::endl ; continue; } // error 
				case 0: { continue; } // not finished
				default:
					client->setCGIStatus(status);
					client->setReadyToresponse(true);
			}
		}
	}
}

void	Server::Forbidden(Client *client) {
	client->getRequest()->setResponsCode("403");
	client->setReadyToresponse(true);
}

void Server::handleDELETE(Client* client) {
	Request	*request = client->getRequest();

	if (request->path().find("..") != std::string::npos) {
		return (Forbidden(client));
	} else if (std::strncmp(request->path().c_str(), "/uploads/", 9) != 0) {
		return (Forbidden(client));
	} else if (request->path() == "/uploads/post.html") {
		return (Forbidden(client));
	} else if (request->path() == "/uploads/" || request->path() == "/uploads") {
		return (Forbidden(client));
	}

	if (access((_data->_root + request->path()).c_str(), F_OK) != 0) {
		client->getRequest()->setResponsCode("404");
		client->setReadyToresponse(true);
		return ;
	}

	if (unlink((_data->_root + request->path()).c_str()) == -1) {
		std::cerr << "DELETE: unlink() failed" << std::endl;
	}

	request->setResponsCode("200");
	client->setReadyToresponse(true);
}

void Server::addClientRequest(int fd) {
	char buff[BUFFER_SIZE];
	int n;
	Client *client;
	static long long sizeBody; // TODO: enleve ca

	client = getClient(fd);
	client->setUseBuffer(true);
	n = recv(fd, buff, BUFFER_SIZE, MSG_DONTWAIT);
	if (n == -1) {
		throw std::runtime_error("Can't recv the message !");
	}
	if (n == 0)
	{
		client->setReadyToresponse(true);
		client->getRequest()->setResponsCode("400");
	}

	if (client->whatToDo() == ON_HEADER && client->isReadyToResponse() == false) {
	 	client->pushHeaderRequest(buff, n);
		client->setUseBuffer(false);
	 	if (client->getReadyToParseHeader()) {
	 		_parseClientHeader(client);
			sizeBody += client->getRequest()->getLenBody();
		}
	}
	if (client->whatToDo() == ON_BODY && client->isReadyToResponse() == false) {
		if (client->getRequest()->isACgi() == true)  // TODO: bouger ca
		{
			if (client->getUseBuffer() == true)
			{
				write(client->getParentToCGI(), buff, n);
				sizeBody += n;
			}
			if (sizeBody == client->getRequest()->getContentLenght())
			{
				close(client->getParentToCGI());
				sizeBody = 0;
			}
		}
		else
			client->getRequest()->addBodyRequest(buff, n, client->getUseBuffer());
	}
	if (client->getRequest()->getMethode() == DELETE_ &&  client->getRequest()->getResponsCode() == "200") {
		handleDELETE(client);
	}
}
