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
	_bondary = NULL;
}

RawBits::~RawBits(void) {
	if (_body)
		delete [] _body;
	if (_request)
		delete [] _request;
}

void RawBits::setBondary(char *str) {
	_bondary = str;
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

#include <cstring>

void	RawBits::checkFileHeader( std::string body, File& file) {
	if (body.find("\r\n") == std::string::npos) {
		throw std::invalid_argument("invalid boundaries: no headers"); //code erreur 400
	}

	for (std::string line = ""; line != "\r\n";) {
		line = body.substr(0, body.find("\r\n") - 1);

		if (line.empty())	{ break; }			/* end of the header */
		else 				{ body.erase(0, body.find("\r\n") + 1); }		/*	erase line from body	*/		// (to be on the next line in the next loop)

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
					if (delim == std::string::npos) { throw std::invalid_argument("invalid boundary: missing value"); }		/*	none of the formats	*/
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

void	RawBits::checkBondaries( void  ) {

	if (_bondary == NULL) {		/* no bondary */
		return ;
	}

	std::string body(_body);
	size_t			bondaryPos = 0;
  std::cerr << "debug boundary : " << _bondary << std::endl;
	const int	bondarySize = std::strlen(_bondary);

	bondaryPos = body.find(_bondary, bondaryPos);	/* no "--" before bondary */
	if (bondaryPos < 2 || body.substr(bondaryPos - 2, 2) != "--") {
		throw std::invalid_argument("invalid bondaries");
	} else if (bondaryPos == std::string::npos) { 	/* bondary inexistant */
		throw std::invalid_argument("invalid bondaries");
	} else if (body.substr(bondaryPos + bondarySize, 2) == "--") { 	/* end bondary as first  */
		throw std::invalid_argument("invalid bondaries");
	}

	size_t			fileStart;
	size_t			fileEnd;

	fileStart = bondaryPos + bondarySize;
	while (true) {
		File 		file;
		fileEnd = body.find(_bondary, fileStart);
		if (fileEnd == std::string::npos) {		/* bondary not found */
			throw std::invalid_argument("invalid bondaries");
		}

		file.content(body.substr(fileStart, body.find(fileEnd)));	/* creating File */
		checkFileHeader(body, file);
		_files.push_back(file);

		bondaryPos = body.find(_bondary, bondaryPos);	/* next bondary */
		fileStart = bondaryPos + bondarySize;

		if (body.compare(fileStart, 2, "--") == 0) {	/* end bondary */
			break;
		}
	}
}

const std::vector<File>& RawBits::getRawFile(void) const {
  return _files;
}
