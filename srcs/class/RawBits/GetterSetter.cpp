/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetterSetter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:41:17 by madamou           #+#    #+#             */
/*   Updated: 2024/11/24 19:42:41 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawBits.hpp"

void RawBits::setBondary(std::string str) {
	_boundary = str;
}

const std::string &RawBits::getHeader(void) const {
	return _header;
} 

const char *RawBits::getBody(void) const {
	return _body;
} 

const char *RawBits::getRequest(void) const {
	return _request;
}

const unsigned int &RawBits::getLenBody(void) const {
	return _lenBody;
}

const long long &RawBits::getLenTotalBody(void) const {
	return _lenTotalBody;
}

void RawBits::setHeader(std::string header) {
	_header = header;
}

File* RawBits::getCurrentFile(void) const {
	return _currentFile;
}