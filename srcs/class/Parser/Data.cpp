/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:35:03 by itahri            #+#    #+#             */
/*   Updated: 2024/12/09 20:05:48 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Parser.hpp"

Data::Data() {
  _allowedMethods = 0;
  _port = 0;
  _autoIndex = false;
  _clientMaxBodySize = -1;
}

Data::~Data() {
  
}

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

Location *Data::checkLocation(std::string path) {
  if (_locations.find(path) != _locations.end())
    return &(_locations.find(path))->second;
  for (std::map<std::string, Location>::iterator it = _locations.begin(); it != _locations.end(); it++) {
    if (!it->first.empty()) {
      if (std::strncmp(it->first.c_str(), path.c_str(), it->first.length()) == 0)
        return &it->second;
    }
  }
  return NULL;
}
