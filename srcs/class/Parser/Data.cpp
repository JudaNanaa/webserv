/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itahri <itahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:35:03 by itahri            #+#    #+#             */
/*   Updated: 2024/11/07 13:40:51 by itahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Parser.hpp"

Data::Data() {
  _allowedMethodes = 0;
  _index = "";
  _root = "";
  _serverName = "";
  _port = 0;
  _cgi = "";
  _location = "";
  _autoIndex = false;
  _errorPage = "";
  _uploadFolder = "";
}

Data::~Data() {}

Data& Data::operator=(const Data& src) {
  if (this != &src) {
    _cgi = src._cgi;
    _port = src._port;
    _root = src._root;
    _index = src._index;
    _location = src._location;
    _autoIndex = src._autoIndex;
    _errorPage = src._errorPage;
    _serverName = src._serverName;
    _uploadFolder = src._uploadFolder;
    _allowedMethodes = src._allowedMethodes;
    _clientMaxBodySize = src._clientMaxBodySize;
  }
  return *this;
}
