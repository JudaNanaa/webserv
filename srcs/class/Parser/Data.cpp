/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:35:03 by itahri            #+#    #+#             */
/*   Updated: 2024/12/14 15:42:04 by madamou          ###   ########.fr       */
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
}

Data::~Data() {
  
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
