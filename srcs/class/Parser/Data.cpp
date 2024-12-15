/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:35:03 by itahri            #+#    #+#             */
/*   Updated: 2024/12/15 18:06:57 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Parser.hpp"
#include <cstddef>

Data::Data() {
  _allowedMethods = 0;
  _port = 0;
  _autoIndex = false;
  _clientMaxBodySize = -1;
  _internal = false;
}

Data::~Data() {
  
}

Data *Data::clone(void) const
{
	Data *dest;

	dest = new Data();
	dest->_portVec = _portVec;
	dest->_port = _port;
	dest->_internal = _internal;
	dest->_serverName = _serverName;
	dest->_root = _root;
    dest->_uploadFolder = _uploadFolder;
	dest->_allowedMethods = _allowedMethods;
	dest->_errorPages =	_errorPages;
	dest->_autoIndex = _autoIndex;	
	dest->_cgi = _cgi;
	dest->_clientMaxBodySize = _clientMaxBodySize;	
	dest->_locations = _locations;
	dest->_index = _index;
	return dest;
}

bool Data::checkLocation(const std::string &path) const {
	if (_locations.find(path) != _locations.end())
		return true;

	std::map<std::string, Location>::const_iterator it, end;
	for (it = _locations.begin(), end = _locations.end(); it != end; it++) {
		if (it->first.length() <= path.length()) {
			std::string location = it->first;
			if (*location.rbegin() == '/')
				*location.rbegin() = '\0';
			if (std::strncmp(location.c_str(), path.c_str(), location.length()) == 0) {
				if (path[location.length()] == '\0' || path[location.length()] == '/')
					return true;
			}
		}
	}
	return false;
}

Location *Data::getLocation(const std::string &path) {
	Location *ok = NULL;
	if (_locations.find(path) != _locations.end())
		return &(_locations.find(path))->second;

	std::map<std::string, Location>::iterator it, end;
	for (it = _locations.begin(), end = _locations.end(); it != end; it++) {
		if (it->first.length() <= path.length()) {
			std::string location = it->first;
			if (*location.rbegin() == '/')
				*location.rbegin() = '\0';
			if (std::strncmp(location.c_str(), path.c_str(), location.length()) == 0) {
				if (path[location.length()] == '\0' || path[location.length()] == '/')
					ok = &it->second;
			}
		}
	}
	return ok;
}

const std::string &Data::getErrorPage(const std::string	&code) const
{
	return _errorPages.at(code);
}

bool Data::errorPageIsSet(const std::string &code) const
{
	return _errorPages.find(code) != _errorPages.end();
}

bool	Data::internal( void ) const {
	return _internal;
}
