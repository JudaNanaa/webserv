/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:35:03 by itahri            #+#    #+#             */
/*   Updated: 2024/12/10 20:28:57 by madamou          ###   ########.fr       */
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
