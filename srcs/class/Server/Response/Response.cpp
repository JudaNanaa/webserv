/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 01:01:30 by madamou           #+#    #+#             */
/*   Updated: 2024/12/15 21:18:41 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

std::string	Server::_getContentType(const std::string& path) {
	if (path.find('.') not_found)
		return ("text/plain");

	std::string extension = path.substr(path.find_last_of('.'));
	return ContentType(extension);
}

std::string	Server::_getResponseHeader(Request *request, const std::string& path) {
	int code = atoi(request->getResponsCode().c_str());

	std::ostringstream oss;
	oss << request->getResponseFileSize();
    std::string header = "HTTP/1.1 " + request->getResponsCode() + " " + getMessageCode(code)+ "\r\n";
    header += "Content-Type: " + _getContentType(path) + "\r\n";
	std::cerr << "Content type: " << _getContentType(path) << std::endl;
    header += "Content-Length: " + oss.str() + "\r\n";
    header += "\r\n";

	return header;
}

void Server::_sendResponse(const int &fd, Client *client) {
	Request *clientRequest = client->getRequest();
	char	buffer[BUFFER_SIZE];
	std::size_t	n = 0;

	std::string	finalPath;
	
	if (clientRequest->responseFileOpen() is false)
	{
		std::cerr << "SEND RESPONSE" << std::endl;
		finalPath = _openResponseFile(clientRequest, client);
		if (clientRequest->getState() is SEND)
			return;
		std::cerr << "RESPONSE CODE : " << clientRequest->getResponsCode() << std::endl;
		std::string	header = _getResponseHeader(clientRequest, finalPath);
		if (send(fd, header.c_str(), header.size(), MSG_NOSIGNAL) is -1) {
			client->setResponse("500");
			throw std::runtime_error("Can't send the message !");
		}
	}

	n += clientRequest->readResponseFile(buffer, BUFFER_SIZE);
	if (send(fd, buffer, n, MSG_NOSIGNAL) is -1)
	{
		clientRequest->closeResponseFile();
		client->setResponse("500");
		throw std::runtime_error("Can't send the message !");
	}
	clientRequest->addResponseFileTotalSend(n);
	if (clientRequest->getResponseFileTotalSend() is clientRequest->getResponseFileSize())
	{
		clientRequest->closeResponseFile();
		clientRequest->setState(SEND);
	}
}

void Server::_sendRedirect(Client *client, const std::string &redirect) {
	Request *clientRequest = client->getRequest();
	int code;

	clientRequest->setResponsCode("302");
	code = atoi(clientRequest->getResponsCode().c_str());
	std::ostringstream oss;
	oss << "HTTP/1.1 " << code << " " << getMessageCode(code) << "\r\n"; 
	oss << "Location: " << redirect << "\r\n";
	oss << "Content-Length: 0\r\n";
	oss << "\r\n";

	std::string response = oss.str();

	if (send(client->getClientFd(), response.c_str(), response.size(), MSG_NOSIGNAL) is -1)
	{
		client->setResponse("500");
		throw std::runtime_error("Can't send the message !");
	}
	clientRequest->setState(SEND);
}

void Server::_sendResponseLocation(Client *client)
{
	Request *clientRequest;
	Location *location;

	clientRequest = client->getRequest();
	location = clientRequest->getLocation();
	if (clientRequest->getRedirect())
	{
		std::cerr << "redirected on : " + location->redirect() << std::endl;
		_sendRedirect(client, location->redirect());
	}
	if (clientRequest->responseCgi() is true)
	{
		if (_checkCgi(client) is FINISHED)
		{
			if (clientRequest->responseCgi() is true)
				_responseCGI(client);
			else
			{
				close(client->getCGIFD());
				client->setCGIFD(-1);
			 	_sendResponse(client->getClientFd(), client);
			}
		}
	}
	else
	 	_sendResponse(client->getClientFd(), client);
}

void Server::_sendResponseDefault(Client *client)
{
	Request *clientRequest;

	clientRequest = client->getRequest();
	if (clientRequest->responseCgi() is true)
	{
		if (_checkCgi(client) is FINISHED)
		{
			if (clientRequest->responseCgi() is true)
				_responseCGI(client);
			else
			{
				close(client->getCGIFD());
				client->setCGIFD(-1);
			 	_sendResponse(client->getClientFd(), client);
			}
		}
	}
	else
	 	_sendResponse(client->getClientFd(), client); //this methode send response with appropriate code
}

t_state Server::giveClientResponse(const int &fd) {
	Client *client;
	std::ifstream file;

	client = _getClient(fd);
	if (std::strncmp(client->getRequest()->path().c_str(), "/auth/", 6) is 0 && client->getRequest()->getResponsCode() is "200")
		_handleAuth(client);
	else if (client->getRequest()->getRequestType() is LOCATION)
		_sendResponseLocation(client);
	else 
		_sendResponseDefault(client);
	if (client->getRequest()->getState() == SEND)
	{
		client->afterResponse();
		return SEND;
	}
	return RESPONSE;
}
