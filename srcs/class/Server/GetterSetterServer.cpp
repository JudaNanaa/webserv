/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetterSetterServer.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:06:52 by madamou           #+#    #+#             */
/*   Updated: 2024/12/28 23:19:21 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/epoll.h>

Client *Server::_getClient(const int &fd) {
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

void	Server::setEnv(char **env) {
	_env = env;
}

char	**Server::getEnv( void ) const {
	return _env;
}

Data *Server::getData(void) const {
	return _data;
}