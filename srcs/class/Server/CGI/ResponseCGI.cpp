/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseCGI.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:18:41 by madamou           #+#    #+#             */
/*   Updated: 2024/11/30 22:02:41 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include <cstddef>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

void Server::_responseCgiIfNoProblem(Client *client)
{
	char *toSend = NULL;
	unsigned long long total = 0;
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
	close(client->getCGIFD());
	if (send(client->getClientFd(), toSend, total, MSG_EOR) == -1)
		throw std::runtime_error("Can't send the message !");
}

void Server::_responseCgiError(Client *client)
{
	std::ostringstream buffer;
	Request *clientRequest = client->getRequest();
	std::ifstream file;

	close(client->getCGIFD());
	file.open(("URIs/errors/" + clientRequest->getResponsCode() + ".html").c_str());
	buffer << file.rdbuf();
	std::string html_content = buffer.str();
	int code = atoi(clientRequest->getResponsCode().c_str());

	std::cerr << "debug code : " << code << std::endl;
	std::ostringstream oss;
    std::string response = "HTTP/1.1 " + clientRequest->getResponsCode() + " " + getMessageCode(code)+ "\r\n";
    response += "Content-Type: text/html\r\n";
	oss << html_content.size();
    response += "Content-Length: " + oss.str() + "\r\n";
    response += "\r\n";
    response += html_content;
	file.close();
	if (send(client->getClientFd(), response.c_str(), response.size(), MSG_EOR) == -1)
		throw std::runtime_error("Can't send the message !");
}

void Server::responseCGI(Client *client)
{
	std::cerr << "-------- RESPONSE CGI --------" << std::endl;
	if (client->getCGIStatus() == EXIT_SUCCESS)
		_responseCgiIfNoProblem(client);
	else
		_responseCgiError(client);
}
