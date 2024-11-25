/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseCGI.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 16:18:41 by madamou           #+#    #+#             */
/*   Updated: 2024/11/25 17:52:13 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

void Server::_responseCgiIfNoProblem(Client *client)
{
	char *toSend = NULL;
	unsigned long long total = 0;
	char buff[BUFFER_SIZE];
	int nbRead = BUFFER_SIZE;

	toSend = strdup("HTTP/1.1 200 Ok\r\n");
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
	write(2, toSend, total);
	if (send(client->getClientFd(), toSend, total, MSG_EOR) == -1)
		throw std::runtime_error("Can't send the message !");
}

void Server::_responseCgiError(Client *client)
{
	(void)client;
	// TODO: Mettre de la logique pour renvoyer une reponse appropriee
}

void Server::responseCGI(Client *client)
{
	if (client->getCGIStatus() == EXIT_SUCCESS)
		_responseCgiIfNoProblem(client);
	else
		_responseCgiError(client);
}
