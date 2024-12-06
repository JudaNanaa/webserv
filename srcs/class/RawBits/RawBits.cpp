/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:57:06 by itahri            #+#    #+#             */
/*   Updated: 2024/11/17 21:48:004 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawBits.hpp"
#include "../Parser/Parser.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

RawBits::RawBits(void)
	: _fileState(ON_HEADER),
	  _body(NULL),
	  _request(NULL),
	  _lenBody(0),
	  _lenTotalBody(0),
	  _lenRequest(0)
{

}


RawBits::~RawBits(void) {
	if (_body)
		delete [] _body;
	if (_request)
		delete [] _request;
}

void RawBits::appendBody(const char *str, const int n) {
	char *dest = new char[_lenBody + n];

	if (_body) {
		std::memmove(dest, _body, _lenBody);
		delete [] _body;
	}
	std::memcpy(&dest[_lenBody], str, n);
	_body = dest;
	_lenBody += n;
	_lenTotalBody += n;
}

void RawBits::BuffToRaw(const char *buff, const int n) {
	char *dest = new char[_lenRequest + n];

	if (_request) {
		std::memmove(dest, _request, _lenRequest);
		delete [] _request;
	}
	std::memcpy(&dest[_lenRequest], buff, n);
	_request = dest;
	_lenRequest += n;
}

long RawBits::find(const char *str) const {
    size_t lenStr = strlen(str);
    for (unsigned int i = 0; i + lenStr <= _lenRequest; i++) {
		if (std::memcmp(&_request[i], str, lenStr) == 0)
            return i;
    }
    return -1;
}

long RawBits::findInBody(const char *str, unsigned long pos) const {
	size_t lenStr = strlen(str);
	if (_lenBody < lenStr)
		return -1;
	for (size_t lenStr = strlen(str); pos <= _lenBody - lenStr; pos++) {
		if (std::memcmp(&_body[pos], str, lenStr) == 0) {
			return pos;
		}
	}
	return -1;
}

void RawBits::splitRequest(void) {
	long sep = find("\r\n\r\n");

	for (unsigned int i = 0; i < sep; i++) {
		_header.push_back(_request[i]);
	}
	sep += 4;
	if (_lenRequest - sep > 0)
		appendBody(&_request[sep], _lenRequest - sep);
	delete [] _request;
	_request = NULL;
}

char* RawBits::substrBody(size_t pos, size_t n) {
	char *result = new char[n + 1];

	std::memcpy(result, &_body[pos], n);
	result[n] = '\0';
	return result;
}

void RawBits::eraseInBody(size_t pos, size_t n) {
	char *newBody = new char[_lenBody - n];

	std::memcpy(newBody, _body, pos);
	std::memcpy(&newBody[pos], &_body[pos + n], _lenBody - (pos + n));
    delete[] _body;
    _body = newBody;
    _lenBody = _lenBody - n;
}
