/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetterSetter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:06:52 by madamou           #+#    #+#             */
/*   Updated: 2024/11/27 01:15:15 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Client *Server::getClient(int fd) {
	return this->_clientMap[fd];
}

int Server::getSocketFd(void) const {
	return this->_socket_fd;
}

void Server::addData(Data* data) {
  _data = data;
}

void Server::addClientToMap(Client *client) {
	this->_clientMap[client->getClientFd()] = client;
}

void	Server::setEnv( char **env ) {
	_env = env;
}

char	**Server::getEnv( void ) const {
	return _env;
}
