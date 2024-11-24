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
  (void)client;
	// TODO
}

void	Server::handleRequest( Client *client ) {
	Request *request = client->getRequest();

	if ((_data->_allowedMethods & request->method()) == 0) {
		request->setResponsCode("405");
		client->setReadyToresponse(true);
		return ;
	}
  //check de la body Size
	if (_data->_clientMaxBodySize > 0) {
		if (_data->_clientMaxBodySize < request->getContentLenght()) {
			request->setResponsCode("413");
			client->setReadyToresponse(true);
			return ;
		}
	}
	if (request->getContentLenght() == -1) {
		client->setReadyToresponse(true);
	}
	if (request->getMethode() & GET_)
		client->setReadyToresponse(true);
}

void	Server::chooseParsing( Client *client ) {
	Request	*request = client->getRequest();

	if (_data->checkLocation(request->path()) != NULL) {
		handleLocation(client);
	} else if (isCgi(request->path()) == true) {
		handleCgi(client);
	} else {
		handleRequest(client);
	}
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
	if (lineSplit[2].compare("HTTP/1.1") != 0) {
		// le htpp nest pas bon !!
		clientRequest->setResponsCode("505");
		return;
	}
	for (std::vector<std::string>::const_iterator it = headerSplit.begin() + 1, ite = headerSplit.end();
			it != ite; it++) {
		_parseRequestLine(*it, clientRequest);
	}
	// std::cout << "REQUEST:\n" << *clientRequest << std::endl;
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

// void Server::_parseClientBody(Client *client) {
// 	std::string filename;
// 	client->getRequest()->printBody();
// 	// TODO: Si path != cgi et pas de multipart form data  alors return instant et repondre 200
// 	// if path request == cgi alors envoye le contenu du body dans l'entree standart du cgi
// 	client->getRequest()->checkBondaries();
// 	std::vector<File*> files = client->getRequest()->getFile();

// 	for (size_t i = 0; i < files.size(); i++) {
// 		filename = generateFilename( files[i]->get("filename")); // need file extension
// 		int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		write(fd, files[i]->content(), files[i]->lenFile());
// 		close(fd);
// 	}

// 	client->setReadyToresponse(true);
// }

void Server::addClientRequest(int fd) {
	char buff[BUFFER_SIZE + 1];
	int n;
	Client *client;

	client = getClient(fd);
	client->setUseBuffer(true);
	n = recv(fd, buff, BUFFER_SIZE, MSG_DONTWAIT);
	if (n == -1) {
		throw std::runtime_error("Can't recv the message !");
	}
	buff[n] = '\0';
	if (client->whatToDo() == ON_HEADER) {
	 	client->pushHeaderRequest(buff, n);
		client->setUseBuffer(false);
	 	if (client->getReadyToParseHeader()) {
	 		_parseClientHeader(client);
		}
	}
	if (client->whatToDo() == ON_BODY && client->isReadyToResponse() == false) {
		client->getRequest()->addBodyRequest(buff, n, client->getUseBuffer());
	}
}
