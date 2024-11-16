/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:57:06 by itahri            #+#    #+#             */
/*   Updated: 2024/11/15 19:51:11 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawBits.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <vector>

RawBits::RawBits(void) {
	_request = NULL;
	_body = NULL;
	_lenBody = 0;
	_lenRequest = 0;
}

RawBits::~RawBits(void) {
	if (_body)
		delete [] _body;
	if (_request)
		delete [] _request;
}

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

void RawBits::appendBody(const char *str, const int n) {
	char *dest = new char[_lenBody + n];
	if (_body) {
		std::memmove(dest, _body, _lenBody);
		delete [] _body;
	}
	std::memcpy(&dest[_lenBody], str, n);
	_body = dest;
	_lenBody += n;
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
    for (unsigned int i = 0; i <= _lenRequest - lenStr; i++) {
        if (std::memcmp(&_request[i], str, lenStr) == 0)
            return i;
    }
    return -1;
}

long RawBits::findInBody(const char *str) const {
  size_t lenStr = strlen(str);
  for (unsigned int i = 0; i <= _lenRequest - lenStr; i++) {
      if (std::memcmp(&_body[i], str, lenStr) == 0)
          return i;
  }
  return -1;
}

long RawBits::findInBody(const char *str, size_t n) const {
  size_t lenStr = strlen(str);
  if (n > _lenBody)
    return -1;
  for (unsigned int i = n; i <= _lenRequest - lenStr; i++) {
      if (std::memcmp(&_body[i], str, lenStr) == 0)
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

void RawBits::printBody(void) const {
	for (unsigned int i = 0; i < _lenBody; i++) {
		std::cout << _body[i];
	}
}

char* RawBits::substrBody(size_t pos, size_t n) {
  std::cerr << "debug alloc len : " << n - pos << std::endl; 
  std::cerr << "debug n : " << n << std::endl; 
  std::cerr << "debug pos : " << pos << std::endl; 
  char *result = new char[n - pos];
  size_t i = 0;
  while (i < n - pos) {
    result[i] = _body[pos + i];
    i++;
  }
  return result;
}

#include <cstring>

void	RawBits::checkFileHeader(File& file) {
	if (findInBody("\r\n") == -1) {
		throw std::invalid_argument("invalid boundaries: no headers"); //code erreur 400
	}

	for (std::string line = ""; line != "\r\n";) {
		line = substrBody(0, findInBody("\r\n") - 1);

		if (line.empty())	{ break; }			/* end of the header */
		else 				{ _body.erase(0, _body.find("\r\n") + 1); }		/*	erase line from body	*/		// (to be on the next line in the next loop)

		for (std::string value; line.empty() == false;) {
			int	headerSize = 0;

			if (std::strncmp(line.c_str(), "name=\"", 6) == 0) {		/* name */
				headerSize = 6 + 1;		// + 1 for the last quote
				value = line.substr(6, line.find("\"", 6) - 1);
				file.name(value);
			} else if (std::strncmp(line.c_str(), "filename=\"", 10) == 0) {	/* filename */
				headerSize = 10 + 1;		// + 1 for the last quote
				value = line.substr(10, line.find("\"", 10) - 1);
				file.filename(value);
			} else if (std::strncmp(line.c_str(), "Content-Disposition: ", 21) == 0) {		/* Content-Disposition */
				headerSize = 21;
				value = line.substr(21, line.find(";") - 1);
				file.contentDisposition(value);
			} else {										/* others */
				std::string	key;
				size_t			delim;
				bool		semicolon = line.find(";") != std::string::npos;	/*	semicolon at the end	*/

				delim = line.find(":");			/* format: "key:value" ? */
				if (delim == std::string::npos) { // wrong format
					delim = line.find("=");			/* format: "key=value" ? */
					if (delim == std::string::npos) { throw std::invalid_argument("invalid boundary: missing value : " + line); }		/*	none of the formats	*/
				}
				key = line.substr(0, delim - 1);
				headerSize = key.size() + 1;	// + 1 for the delimiter
				if (semicolon == true)	{ value = line.substr(delim + 1, line.find(";") - 1); }
				else					{ value = line.substr(delim + 1); }

				file.set(key, value);
			}

			line.erase(0, headerSize + value.size());

			if (line[0] == ';')					{ line.erase(0, 1); }	/* ; after the value */
			else if (line.empty() == false)		{ throw std::invalid_argument("invalid boundary headers"); }	/* no ; after value but line not empty */

		}
	}

}

// std::size_t	nextBoundary(std::string& body) {
// 	std::size_t pos = 0;
//
// 	while (true) {
// 		pos = body;
// 	}
// }


int RawBits::compareInBody(char *s, size_t n) {
  return memcmp(_body, s, n);
}

void	RawBits::checkBondaries( void  ) {

	if (_boundary.empty()) {		/* no bondary */
		return ;
	}

	size_t			boundaryPos = 0;
  std::cerr << "debug boundary : " << _boundary << std::endl;
	const int	boundarySize = _boundary.size();

	// bondaryPos = findInBody(_bondary, bondaryPos);	/* no "--" before bondary */
  boundaryPos = findInBody(_boundary.c_str(), boundaryPos);
	if (boundaryPos < 2 || std::memcmp(substrBody(boundaryPos - 2, 2), "--", 2) == 0) {
		throw std::invalid_argument("invalid bondaries 1");
	} else if (std::memcmp(substrBody(boundaryPos - 2, 2), "--", 2) == 0) { 	/* end bondary as first  */
		throw std::invalid_argument("invalid bondaries 2");
	}

	size_t			fileStart;
	long			fileEnd;

	fileStart = boundaryPos + boundarySize;
  std::cerr << "file start " << fileStart << std::endl;
	while (true) {
		File 		file;
		fileEnd = findInBody(_boundary.c_str(), fileStart);
		if (fileEnd == -1) {		/* bondary not found */
			throw std::invalid_argument("invalid bondaries 3");
		}

		file.content(substrBody(fileStart, fileEnd));	/* creating File */
		checkFileHeader(file);
		_files.push_back(file);

		boundaryPos = findInBody(_boundary.c_str(), boundaryPos);	/* next bondary */
		fileStart = boundaryPos + boundarySize;

		if (findInBody("--", fileStart) != -1) {	/* end bondary */
			break;
		}
	}
}

const std::vector<File>& RawBits::getRawFile(void) const {
  return _files;
}
