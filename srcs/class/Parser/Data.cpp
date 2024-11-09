/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:35:03 by itahri            #+#    #+#             */
/*   Updated: 2024/11/08 20:29:01 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Parser.hpp"

Data::Data() {
  _allowedMethods = 0;
  _index = "";
  _root = "";
  _serverName = "";
  _port = 0;
  _cgi = "";
  _autoIndex = false;
  _errorPage = "";
  _uploadFolder = "";
  _clientMaxBodySize = 0;
}

Data::~Data() {}

Data& Data::operator=(const Data& src) {
  if (this != &src) {
    _cgi = src._cgi;
    _port = src._port;
    _root = src._root;
    _index = src._index;
    _autoIndex = src._autoIndex;
    _errorPage = src._errorPage;
    _serverName = src._serverName;
    _uploadFolder = src._uploadFolder;
    _allowedMethods = src._allowedMethods;
    _clientMaxBodySize = src._clientMaxBodySize;
	for (std::map<std::string, Location>::const_iterator it = src._locations.begin(), ite = src._locations.end();
			it != ite; it++) {
		_locations[it->first] = it->second;
	}
  }
  return *this;
}
