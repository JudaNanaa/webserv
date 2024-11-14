/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:57:06 by itahri            #+#    #+#             */
/*   Updated: 2024/11/05 14:57:14 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawBits.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>

RawBits::RawBits(void) {
	_request = NULL;
	_body = NULL;
	_lenBody = 0;
	_lenRequest = 0;
}

RawBits::~RawBits(void) {
	// std::cerr << "test moussa" << std::endl;
	// if (_content)
	// 	delete[] _content;
	// _content = NULL;
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

void RawBits::appendBody(const char *str, const int n) {
	char *dest = new char[_lenBody + n];
	if (_body)
		std::memmove(dest, _body, _lenBody);
	std::memcpy(&dest[_lenBody], str, n);
	delete [] _body;
	_body = dest;
	_lenBody += n;
	std::cerr <<  std::endl << "len body == " << _lenBody << std::endl;
}

void RawBits::BuffToRaw(const char *buff, const int n) {
	char *dest = new char[_lenRequest + n];
	if (_request)
		std::memmove(dest, _request, _lenRequest);
	std::memcpy(&dest[_lenRequest], buff, n);
	delete [] _request;
	_request = dest;
	_lenRequest += n;
}

long RawBits::find(const char *str) const {
	for (unsigned int i = 0; i < _lenRequest - strlen(str); i++) {
		if (std::memcmp(&_request[i], str, strlen(str)) == 0)
			return i;
	}
	return -1;
}

void RawBits::setHeader(std::string header) {
	_header = header;
}

void RawBits::splitRequest(void) {
	long sep = find("\r\n\r\n");

	for (unsigned int i = 0; i < sep; i++) {
		_header.push_back(_request[i]);
	}
	sep += 4;
	if (_lenRequest - sep > 0)
		appendBody(&_request[sep], _lenRequest - sep);
}
