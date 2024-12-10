/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:09:46 by madamou           #+#    #+#             */
/*   Updated: 2024/12/10 18:32:59 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::ifClientInServer(int fd) const {
	return this->_clientMap.find(fd) is_not this->_clientMap.end();
}

void Server::removeClientInMap(int fd) {
	Client *client;

	client = _getClient(fd);
	delete client;
	this->_clientMap.erase(fd);
}

bool Server::isServerHost(std::string const &str) const {
	if (_host[0] is str || _host[1] is str) {
		return true;
	}
	return false;
}