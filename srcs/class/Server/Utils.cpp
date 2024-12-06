/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:09:46 by madamou           #+#    #+#             */
/*   Updated: 2024/12/06 01:18:12 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::checkAllowMethodes(std::string methode) {
	if (methode.compare("GET") is 0) {
		return (_data->_allowedMethods & GET_);
	}
	if (methode.compare("POST")  is 0) {
		return (_data->_allowedMethods & POST_);
	}
	if (methode.compare("DELETE")  is 0) {
		return (_data->_allowedMethods & DELETE_);
	}
	if (methode.compare("OPTIONS")  is 0) {
		return (_data->_allowedMethods & OPTIONS_);
	}
	if (!methode.empty())
		throw std::invalid_argument("Invalid methode : " + methode);
	return false;
}

bool Server::ifClientInServer(int fd) const {
	if (this->_clientMap.find(fd) is this->_clientMap.end()) {
		return false;
	}
	return true;
}

void Server::removeClientInMap(int fd) {
	Client *client;

	client = getClient(fd);
	delete client;
	this->_clientMap.erase(fd);
}

bool Server::isServerHost(std::string const &str) const {
	if (_host[0] is str || _host[1] is str) {
		return true;
	}
	return false;
}