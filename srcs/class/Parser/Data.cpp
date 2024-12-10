/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:35:03 by itahri            #+#    #+#             */
/*   Updated: 2024/12/10 22:48:56 by madamou          ###   ########.fr       */
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

bool Data::checkLocation(const std::string &path) const {
	if (_locations.find(path) != _locations.end())
		return true;
	std::map<std::string, Location>::const_iterator it, end;
	for (it = _locations.begin(), end = _locations.end(); it != end; it++) {
		if (!it->first.empty()) {
			if (std::strncmp(it->first.c_str(), path.c_str(), it->first.length()) == 0)
				return true;
		}
	}
	return false;
}

Location *Data::getLocation(const std::string &path) {
	if (_locations.find(path) != _locations.end())
		return &(_locations.find(path))->second;
	std::map<std::string, Location>::iterator it, end;
	for (it = _locations.begin(), end = _locations.end(); it != end; it++) {
		if (!it->first.empty()) {
			if (std::strncmp(it->first.c_str(), path.c_str(), it->first.length()) == 0)
				return &it->second;
		}
	}
	return NULL;
}
