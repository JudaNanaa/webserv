/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:09:46 by madamou           #+#    #+#             */
/*   Updated: 2024/12/13 11:34:25 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::ifClientInServer(const int &fd) const {
	return this->_clientMap.find(fd) is_not this->_clientMap.end();
}

void Server::removeClientInMap(const int &fd) {
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

bool	Server::isCgi( const std::string& path ) {
	std::size_t	extension;

	extension = path.find_last_of('.');
	if (extension not_found)
		return (false);
	else if (_data->_cgi.find(path.substr(extension)) is _data->_cgi.end())
		return (false);
	return (true);
}

bool	Server::_isLocation(const std::string &path)
{
	return _data->checkLocation(path);
}

bool Server::_checkLocationCgi(Location* location, const std::string &extension) {
	if (location->cgi().empty() || location->cgi().find(extension) == location->cgi().end())
		return false;
	return true;
}
