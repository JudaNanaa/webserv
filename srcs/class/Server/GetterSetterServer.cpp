/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetterSetter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:06:52 by madamou           #+#    #+#             */
/*   Updated: 2024/12/10 18:44:34 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Client *Server::_getClient(int fd) {
	return this->_clientMap[fd];
}

int const &Server::getSocketFd(void) const {
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
