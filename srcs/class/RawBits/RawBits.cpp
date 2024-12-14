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

void RawBits::_appendBody(const char *str, const int &n) {
	char *dest = new char[_lenBody + n];

	if (_body) {
		std::memmove(dest, _body, _lenBody);
		delete [] _body;
	}
	std::memmove(&dest[_lenBody], str, n);
	_body = dest;
	_lenBody += n;
	_lenTotalBody += n;
}

void RawBits::BuffToRequest(const char *buff, const size_t &n) {
	char *dest = new char[_lenRequest + n];

	if (_request) {
		std::memmove(dest, _request, _lenRequest);
		delete [] _request;
	}
	std::memmove(&dest[_lenRequest], buff, n);
	_request = dest;
	_lenRequest += n;
}

long RawBits::_findInRequest(const char *str) const {
    size_t lenStr = strlen(str);

    for (unsigned int i = 0; i + lenStr <= _lenRequest; i++) {
		if (std::memcmp(&_request[i], str, lenStr) == 0)
            return i;
    }
    return -1;
}

long RawBits::findInBody(const char *str, const std::size_t &start) const {
	size_t lenStr = strlen(str);

	if (_lenBody < lenStr)
		return -1;
	std::vector<int> index_table(lenStr, 0);
	
	for (std::size_t i = 1; i < lenStr; i++) {
		for (std::size_t j = 0; str[i] == str[j]; j++, i++)
			index_table[i] = j + 1;
	}

	std::size_t j = 0;
	std::size_t pos = start;
	while (pos < _lenBody)
	{
		if (_body[pos] == str[j])
			j++;
		else if (_body[pos] != str[j] && j > 0) {
			j = index_table[j - 1];
			pos--;
		}
		pos++;
		if (j == lenStr)
			return pos - j;
	}

	return -1;
}

void RawBits::splitRequest(void) {
	long sep = _findInRequest("\r\n\r\n");

	for (unsigned int i = 0; i < sep; i++)
		_header.push_back(_request[i]);
	sep += 4;
	if (_lenRequest - sep > 0)
		_appendBody(&_request[sep], _lenRequest - sep);
	delete [] _request;
	_request = NULL;
}

char* RawBits::_substrBody(const size_t &pos, const size_t &n) {
	char *result = new char[n + 1];

	std::memmove(result, &_body[pos], n);
	result[n] = '\0';
	return result;
}

void RawBits::eraseInBody(const size_t &pos, const size_t &n) {
	if (n > _lenBody - pos)
	{
		delete [] _body;
		_body = NULL;
		_lenBody = 0;
		return;
	}
	char *newBody = new char[_lenBody - n];

	std::memmove(newBody, _body, pos);
	std::memmove(&newBody[pos], &_body[pos + n], _lenBody - (pos + n));
    delete [] _body;
    _body = newBody;
    _lenBody -= n;
}
