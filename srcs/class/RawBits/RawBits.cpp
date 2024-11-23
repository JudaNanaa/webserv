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

RawBits::RawBits(void) {
	_request = NULL;
	_state = ON_HEADER;
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

void RawBits::cleanFiles(void) {
	for (std::vector<File *>::iterator it = _files.begin(); it != _files.end(); it++) {
		(*it)->freeContent();
		delete *it;
	}
}

void	RawBits::deleteTmp(void) {
	delete [] _tmp;
	_tmp = NULL;
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
	std::memset(dest, 0, sizeof(char) * _lenBody + n);
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

long RawBits::findInBody(const char *str, unsigned long pos) const {
	size_t lenStr = strlen(str);

	std::cerr << "DBG[" << _body << "]" << std::endl;
	for (; pos <= _lenBody - lenStr; pos++) {
		if (std::memcmp(&_body[pos], str, lenStr) == 0)
			return pos;
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
  char *result = new char[n + 1];
  size_t i = 0;
  while (i < n) {
    result[i] = _body[pos + i];
    i++;
  }
  result[i] = '\0';
  return result; 
}

void RawBits::eraseInBody(size_t pos, size_t n) {
  char *newBody = new char[_lenBody - (n - pos)];
  for (size_t i = pos; i < pos + n; i++) {
	newBody[i] = _body[i];
  }
  if (_body)
  	delete []_body;
  _body = newBody;
}

void	RawBits::checkFileHeader(File& file, std::string &header) {
	// for the "\r\n"
	std::cerr << "----HEADER----" << std::endl << header << std::endl;
	std::vector<std::string> fileHeaderSplit = split(header, "\r\n");

	for (std::vector<std::string>::iterator it = fileHeaderSplit.begin(); it != fileHeaderSplit.end(); it++) {
		std::vector<std::string> elements = split(*it, ";");
		for (std::vector<std::string>::iterator it2 = elements.begin(); it2 != elements.end(); it2++) {
			std::vector<std::string> info;
			if (it2 == elements.begin())
				info = split(*it2, ":");
			else
				info = split(*it2, "=");
			std::cerr << "info: " << *it2 << std::endl;
			if (info.size() != 2) {
				std::cerr << "bad info 1" << std::endl;
				continue ;
			}
			// std::cerr << "info[0] '" << trim(info[0].) << "' | " << "info[1] '"<< trim(info[1]) + "'" << std::endl; 
			file.set(trim(info[0]), trim(info[1]));
		}
	}
	std::cout << "Content-Disposition : " << file.get("Content-Disposition") << std::endl;
	std::cout << "name : " << file.get("name") << std::endl;
	std::cout << "filename : " << file.get("filename") << std::endl;
	std::cout << "Content-Type : " << file.get("Content-Type") << std::endl;
}

int RawBits::compareInBody(char *s, size_t n) {
  return memcmp(_body, s, n);
}

void	RawBits::flushBuffer( long pos, std::ofstream& uploadFile, long n ) {
	if (uploadFile.fail()) {
		throw std::invalid_argument("failed to open " + _files.back()->get("filename"));
	} else {
		// file.clear();	(?)
		uploadFile.write(&_body[pos], n);
	}
}

void	RawBits::flushBuffer( std::string& buff, std::ofstream& uploadFile ) {
	if (uploadFile.fail()) {
		throw std::invalid_argument("failed to open " + _files.back()->get("filename"));
	} else {
		// uploadFile.clear();	(?)
		uploadFile << buff;
	}
}

int	RawBits::handleFileHeader( std::ofstream& uploadFile ) {
	long		headerEnd;
	long		fileStart;
	std::string	header;

	fileStart = findInBody(("--" + _boundary).c_str()) + _boundary.size() + 4;
	headerEnd = findInBody("\r\n\r\n", fileStart);
	if (headerEnd == -1) {	/* header incomplete */
		return STOP;
	}

	header = substrBody(fileStart, headerEnd - fileStart);
	// std::cerr << "header = ["<< header << "]"<< std::endl;
	File 		*file = new File();
	checkFileHeader(*file, header);
	_files.push_back(file);

	// delete already handled content
	eraseInBody(0, headerEnd + 4);	// +4 for "\r\n\r\n"

	// flushBuffer(header);	// can throw

	uploadFile.open(("URIs/uploads/" + file->get("filename")).c_str());
	if (uploadFile.fail())
		throw std::invalid_argument("failed to open " + file->get("filename"));

	_state = ON_BODY;
	return (CONTINUE);
}

int	RawBits::checkBondaries( void  ) {
	std::ofstream	uploadFile;

	if (_boundary.empty()) {		/* no bondary */
		// TODO: ?
		std::cerr << "ERROR: no bondary" << std::endl;
		return STOP;
	}

	std::cerr << "debug boundary : " << _boundary << std::endl;

	while (true) {
		if (_state == ON_HEADER) {
			if (handleFileHeader(uploadFile) == STOP)
				return (NOT_FINISHED);
		} if (_state == ON_BODY) {
			long boundaryPos = findInBody(("--" + _boundary).c_str(), _boundary.size() + 2);

			if (boundaryPos == -1) {
				flushBuffer(0, uploadFile, _lenBody - (_boundary.size() + 2));		// in case a part of bondary is at the end
				eraseInBody(0, _lenBody - (_boundary.size() + 2));
				return (NOT_FINISHED);
			} else {
				flushBuffer(0, uploadFile, _lenBody - boundaryPos - 1);
				eraseInBody(0, _lenBody - boundaryPos - 1);
				uploadFile.close();
				if (&_body[boundaryPos] == ("--" + _boundary + "--")) {		// end bondary
					_state = ON_HEADER;
					return (FINISHED);
				}
				_state = ON_HEADER;
				continue;
			}
		}
	}

}

// void	RawBits::checkBondaries( void  ) {		/*		ORIGINAL	*/

// 	if (_boundary.empty()) {		/* no bondary */
// 		return ;
// 	}
// 	size_t			boundaryPos = 0;
// 	std::cerr << "debug boundary : " << _boundary << std::endl;
// 	const int	boundarySize = _boundary.size();

// 	// bondaryPos = findInBody(_bondary, bondaryPos);	/* no "--" before bondary */
//   	boundaryPos = findInBody(_boundary.c_str(), boundaryPos);
// 	_tmp = substrBody(boundaryPos - 2, 2);
// 	if (boundaryPos < 2 || std::memcmp(_tmp, "--", 2) != 0) {
// 		throw std::invalid_argument("invalid bondaries 1");
// 	}
// 	deleteTmp();
// 	_tmp = substrBody(boundaryPos + boundarySize, 2);
// 	if (std::memcmp(_tmp, "--", 2) == 0) { 	/* end bondary as first  */
// 		throw std::invalid_argument("invalid bondaries 2");
// 	}
// 	deleteTmp();
// 	size_t			fileStart;
// 	long			fileEnd;
// 	std::string header;

// 	while (true) {
// 		fileStart = findInBody("\r\n\r\n", boundaryPos + boundarySize + 2) + 4;
// 		if (std::memcmp(&_body[fileStart - 2], "--", 2) == 0) {	/* end bondary */
// 			break;
// 		}
// 		fileEnd = findInBody(_boundary.c_str(), fileStart) - 4;
// 		_tmp = substrBody(boundaryPos + boundarySize + 2, (fileStart - 4) - (boundaryPos + boundarySize + 2));
// 		header = _tmp;
// 		std::cerr << "header = ["<< header << "]"<< std::endl;
// 		File 		*file = new File();
// 		if (fileEnd == -1) {		/* bondary not found */
// 			throw std::invalid_argument("invalid bondaries 3");
// 		}

// 		file->content(substrBody(fileStart, fileEnd - fileStart));	/* creating File */
// 		file->lenFile(fileEnd - fileStart);
// 		checkFileHeader(*file, header);
// 		deleteTmp();
// 		_files.push_back(file);

// 		boundaryPos = findInBody(_boundary.c_str(), fileEnd + 2);	/* next bondary */
// 		// fileStart = boundaryPos + boundarySize;
// 	}
// }

const std::vector<File*>& RawBits::getRawFile(void) const {
  return _files;
}
