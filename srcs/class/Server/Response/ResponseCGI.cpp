/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseCGI.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:18:41 by madamou           #+#    #+#             */
/*   Updated: 2024/12/15 20:02:39 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include <cstddef>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

char *Server::_checkContentLength(char *toSend, std::size_t &total)
{
	std::size_t i;
	std::string header;
	char *dest;
	std::ostringstream oss;

	i = 0;
	while (toSend[i])
	{
		if (std::memcmp(&toSend[i], "\r\n\r\n", 4) == 0 || std::memcmp(&toSend[i], "\n\n",2) == 0)
			break;
		i++;
	}
	for (std::size_t j = 0; j < i; j++)
		header.push_back(toSend[j]);
	if (header.find("Content-Length: ") is_found)
		return toSend;

	oss << total - i - 4;
	std::string line = "\r\nContent-Length: " + oss.str();
	header += line;
	std::size_t len = total + line.length();
	dest = new char[len];
	std::memcpy(dest, header.data(), header.length());
	std::memcpy(&dest[header.length()], &toSend[i], total - i);
	delete [] toSend;
	total = len;
	return dest;
}

void Server::_responseCgiIfNoProblem(Client *client)
{
	char *toSend = NULL;
	std::size_t total = 0;
	char buff[BUFFER_SIZE];
	int nbRead = BUFFER_SIZE;

	toSend = new char[19];
	std::memcpy(toSend, "HTTP/1.1 200 Ok\r\n\0", 19);
	total = strlen(toSend);
	while (nbRead == BUFFER_SIZE)
	{
		nbRead = read(client->getCGIFD(), buff, BUFFER_SIZE);
		if (nbRead == -1)
		{
			close(client->getCGIFD());
			throw std::runtime_error("error read fd cgi !");
		}
		if (nbRead == 0)
			break;
		char *dest = new char[total + nbRead];
		std::memcpy(dest, toSend, total);
		std::memcpy(&dest[total], buff, nbRead);
		delete [] toSend;
		toSend = dest;
		total += nbRead;
	}
	toSend = _checkContentLength(toSend, total);
	close(client->getCGIFD());
	if (send(client->getClientFd(), toSend, total, MSG_NOSIGNAL) == -1)
	{
		delete [] toSend;
		throw std::runtime_error("Can't send the message !");
	}
	delete [] toSend;
}

void Server::_responseCgiError(Client *client)
{
	std::ostringstream buffer;
	Request *clientRequest = client->getRequest();
	std::ifstream file;
	std::string responseCode;


	close(client->getCGIFD());
	responseCode = clientRequest->getResponsCode();
	if (clientRequest->getRequestType() is LOCATION)
	{
		Location *location = _data->getLocation(clientRequest->path());
		if (location->errorPageIsSet(responseCode) is true)
		{
			_sendRedirect(client, location->getErrorPage(responseCode));
			return;
		}
	}
	if (_data->errorPageIsSet(responseCode) is true)
	{
		_sendRedirect(client, _data->getErrorPage(responseCode));
		return;
	}
	file.open(("URIs/errors/" + clientRequest->getResponsCode() + ".html").c_str());
	buffer << file.rdbuf();
	std::string html_content = buffer.str();
	int code = atoi(clientRequest->getResponsCode().c_str());

	std::ostringstream oss;
    std::string response = "HTTP/1.1 " + clientRequest->getResponsCode() + " " + getMessageCode(code)+ "\r\n";
    response += "Content-Type: text/html\r\n";
	oss << html_content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
    response += "\r\n";
    response += html_content;
	file.close();
	if (send(client->getClientFd(), response.c_str(), response.size(), MSG_NOSIGNAL) == -1)
		throw std::runtime_error("Can't send the message !");
}

void Server::_responseCGI(Client *client)
{
	std::cerr << "-------- RESPONSE CGI --------" << std::endl;
	if (client->getCGIStatus() == EXIT_SUCCESS)
		_responseCgiIfNoProblem(client);
	client->getRequest()->setState(SEND);
}
