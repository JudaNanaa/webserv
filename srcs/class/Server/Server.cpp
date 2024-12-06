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
#include <iterator>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <unistd.h>
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
	// Open socket
	this->_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_socket_fd is -1) {
		throw std::runtime_error("Can't open socket");
	}
	
	if(setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) is -1) { 
        throw std::runtime_error("Could not set socket options");	
    }
 
	if(setsockopt(this->_socket_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) is -1) { 
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

	if (listen(this->_socket_fd, INT_MAX) < 0) {
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

  if ((location->allowedMethods() & request->method()) is 0) {
    client->setResponse("405");
    return ;
  }
  //check de la body Size
	if (location->maxBodySize() > 0) {
		if ((unsigned)location->maxBodySize() < request->getContentLenght()) {
    		client->setResponse("413");
    		return ;
		}
	}
	if (request->getContentLenght() is -1) // no body
    		client->setResponse();
}

bool	Server::isCgi( const std::string& path ) {
	std::size_t	extension;

	extension = path.find_last_of('.');
	if (extension not_found)
		return (false);
	else if (_data->_cgi.find(path.substr(extension)) is _data->_cgi.end())
		return (false);
	return (true);
}

void	Server::handleCgi( Client *client ) {
	client->getRequest()->setIsACgi(true);
	CgiDefaultGesture(client);
}

void	Server::handleRequest( Client *client ) {
	Request *request = client->getRequest();

	if ((_data->_allowedMethods & request->method()) is 0) {
		client->setResponse("405");
		return ;
	}
	//check de la body Size
	if (_data->_clientMaxBodySize >= 0) {
		if (_data->_clientMaxBodySize < request->getContentLenght()) {
			client->setResponse("413");
			return ;
		}
	}
	if (request->getContentLenght() is -1) {
		client->setResponse();
	}
}

void	Server::chooseParsing( Client *client ) {
	Request	*request = client->getRequest();

	if (_data->checkLocation(request->path()) != NULL) {
		std::cerr << "LOCATION" << std::endl;
		handleLocation(client);
	} else if (isCgi(request->path()) is true) {
		std::cerr << "CGI" << std::endl;
		handleCgi(client);
	} else {
		std::cerr << "DEFAULT" << std::endl;
		handleRequest(client);
	}
	request->setStatus(ON_BODY);
}

void	Server::_addHeaderLine( std::string line, Request *clientRequest) {

	if (line.find(": ") not_found)
		throw std::invalid_argument("invalid line: " + line);

	std::string	value = line.substr(line.find(": ") + 2);
	std::string key = line.erase(line.find(": "));	// erase the value (keep the key)
	clientRequest->addHeaderLineToMap(key, value);
}

void Server::_parseOtherLinesHeader(Client *client, const std::vector<std::string> &headerSplit)
{
	Request *clientRequest = client->getRequest();

	std::vector<std::string>::const_iterator it;
	for (it = headerSplit.begin() + 1; it != headerSplit.end(); it++) {
		try {
			_addHeaderLine(*it, clientRequest);
		} catch (std::exception &e) {
			client->setResponse("400");
			throw std::invalid_argument(e.what());
		}
	}
}

void Server::_parseFirstLineHeader(Client *client, const std::vector<std::string> &headerSplit)
{
	Request *clientRequest = client->getRequest();
	std::string header = clientRequest->getHeader();
	std::vector<std::string> lineSplit;

	if (std::count(headerSplit[0].begin(), headerSplit[0].end(), ' ') is_not 2) {
		// La premiere ligne est pas bonne donc faire une reponse en fonction
		client->setResponse("400");
		throw std::invalid_argument("Error header 1: " + headerSplit[0]);
	}
	lineSplit = split(headerSplit[0], " ");
	if (lineSplit.size() != 3) {
		client->setResponse("400");
		throw std::invalid_argument("Error header 2: " + headerSplit[0]);
	}
	clientRequest->setMethode(lineSplit[0]);
	clientRequest->path(lineSplit[1]);
	if (lineSplit[2].compare(0, 6, "HTTP/1") != 0) {
		client->setResponse("505");
		throw std::invalid_argument("Error header 3: " + headerSplit[0]);
	}
}

void Server::_parseContentLengthAndBoundary(Request *clientRequest)
{
	if (clientRequest->isKeyfindInHeader("Content-Length") is true) {
		clientRequest->setSizeBody(std::atoll(clientRequest->find("Content-Length").c_str()));
		if (clientRequest->isKeyfindInHeader("Content-Type")) {
			if (clientRequest->find("Content-Type").find("boundary") is_found) {
				std::string bondary = clientRequest->find("Content-Type").substr(clientRequest->find("Content-Type").find("boundary") + 9);
				clientRequest->setBondary(bondary.c_str());
			}
		}
	}
}

void Server::_parseClientHeader(Client *client) {
	Request *clientRequest = client->getRequest();
	std::string header = clientRequest->getHeader();
	std::vector<std::string> headerSplit;
	std::vector<std::string> lineSplit;

	headerSplit = split(header, "\r\n");
	_parseFirstLineHeader(client, headerSplit); // Parse la 1ere ligne du header
	_parseOtherLinesHeader(client, headerSplit); // Parse les autres lignes

	std::cerr << *clientRequest << std::endl; // Print Request

	_parseContentLengthAndBoundary(clientRequest); // set content length et boundary
  	chooseParsing(client); // apre avoir recuperer les infos, on choisie le parsing approprier grace aux informations recuperer
}

void Server::checkCgi( void ) {
	std::map<int, Client*>::iterator it, ite;

	it = _clientMap.begin();
	ite = _clientMap.end();

	for (;it != ite; it++) {
		Client	*client = it->second;
		int		pid = client->getPid();

		if (pid is -1) {	// no 
			continue ;
		} else {
			int	status;
			switch (waitpid(pid, &status, WNOHANG)) {
				case -1: { std::cerr << "waitpid failed" << std::endl ; continue; } // error 
				case 0: { continue; } // not finished
				default:
					client->setResponse();
					client->setCGIStatus(status);
			}
		}
	}
}

void Server::handleDELETE(Client* client) {
	Request	*request = client->getRequest();

	if (request->path().find("..") not_found) {
		return (client->setResponse("403"));
	} else if (std::strncmp(request->path().c_str(), "/uploads/", 9) != 0) {
		return (client->setResponse("403"));
	} else if (request->path() is "/uploads/post.html") {
		return (client->setResponse("403"));
	} else if (request->path() is "/uploads/" || request->path() is "/uploads") {
		return (client->setResponse("403"));
	}

	if (access((_data->_root + request->path()).c_str(), F_OK) != 0) {
		client->setResponse("404");
		return ;
	}

	if (unlink((_data->_root + request->path()).c_str()) is -1) {
		std::cerr << "DELETE: unlink() failed" << std::endl;
	}
	client->setResponse("200");
}

void Server::writeBodyToCgi(Client *client, char *buff, int n)
{
	Request *clientRequest = client->getRequest();
	int result;

	if (client->getUseBuffer() is true)
	{
		result = write(client->getParentToCGI(), buff, n);
		clientRequest->incrementSizeBody(n);
	}
	else
		result = write(client->getParentToCGI(), clientRequest->getBody(), clientRequest->getLenBody());
	(void)result;
	if (clientRequest->getLenTotalBody() is clientRequest->getContentLenght() || clientRequest->getContentLenght() is -1)
	{
		close(client->getParentToCGI());
		client->setParentToCGI(-1);
	}
}

void Server::addClientRequest(int fd) {
	char buff[BUFFER_SIZE];
	int n;
	Client *client = getClient(fd);
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
	if (client->whatToDo() is ON_HEADER) {
		try {
	 		client->pushHeaderRequest(buff, n);
		} catch (std::exception &e) {
			client->setResponse("505");
			throw std::runtime_error(e.what());
		}
		client->setUseBuffer(false);
	 	if (client->getReadyToParseHeader())
	 		_parseClientHeader(client);
	}
	if (client->whatToDo() is ON_BODY) {
		if (clientRequest->isACgi() is true)
			writeBodyToCgi(client, buff, n);
		else
			clientRequest->addBodyRequest(buff, n, client->getUseBuffer());
	}
	if (clientRequest->getMethode() is DELETE_ && clientRequest->getResponsCode() is "200") {
		handleDELETE(client);
	}
}
