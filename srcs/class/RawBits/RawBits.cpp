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
	_fileState = ON_HEADER;
	_body = NULL;
	_lenBody = 0;
	_lenTotalBody = 0;
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

const long long &RawBits::getLenTotalBody(void) const {
	return _lenTotalBody;
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
    for (unsigned int i = 0; i <= _lenRequest - lenStr; i++) {
		if (std::memcmp(&_request[i], str, lenStr) == 0)
            return i;
    }
    return -1;
}

long RawBits::findInBody(const char *str, unsigned long pos) const {
	size_t lenStr = strlen(str);

	for (; pos <= _lenBody - lenStr; pos++) {
		if (std::memcmp(&_body[pos], str, lenStr) == 0) {
			return pos;
		}
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
    if (pos + n > _lenBody) {
        n = _lenBody - pos;
    }
    char *newBody = new char[_lenBody - n];
    size_t i = 0;
    for (size_t j = 0; j < _lenBody; j++) {
        if (j < pos || j >= pos + n) {
            newBody[i++] = _body[j];
        }
    }
    delete[] _body;
    _body = newBody;
    _lenBody = _lenBody - n;
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
			if (info.size() != 2) {
				std::cerr << "bad info: " << *it2 << std::endl;
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

void	RawBits::flushBuffer( long pos, long n ) {
	if (_uploadFile.fail()) {
		throw std::invalid_argument("failed to open " + _files.back()->get("filename"));
	} else {
		_uploadFile.write(&_body[pos], n);
	}
}

int	RawBits::handleFileHeader(void) {
	long		headerEnd;
	long		fileStart;
	std::string	header;

	fileStart = findInBody(("--" + _boundary).c_str()) + _boundary.size() + 4; // +4 pour sauter /r/n/r/n
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

	std::string filename = file->get("filename");
	filename.erase(0, 1);
	filename.erase(filename.size() - 1, 1);
	_uploadFile.open(("URIs/uploads/" + filename).c_str());
	if (_uploadFile.fail())
		throw std::invalid_argument("failed to open " + filename);
	_fileState = ON_BODY;
	return (CONTINUE);
}

int    RawBits::handleFileBody(void) {
    long boundaryPos = findInBody(("--" + _boundary).c_str());

    if (boundaryPos == -1) { // On a pas encore trouve la bondary qui dit que le fichier est fini
        flushBuffer(0, _lenBody - (_boundary.size() + 2));        // in case a part of bondary is at the end
        eraseInBody(0, _lenBody - (_boundary.size() + 2));
        return (NOT_FINISHED);
    } else if (std::memcmp(&_body[boundaryPos], ("--" + _boundary + "--").c_str(), _boundary.size() + 4) == 0) {    // end bondary
        flushBuffer(0, boundaryPos - 2);
        // eraseInBody(0, boundaryPos + _boundary.size() + 4 + 4); // TODO: maybe change this / check condition
		delete [] _body;
		_body = NULL;
        _fileState = ON_HEADER;
        _uploadFile.close();
        return (FINISHED);
    } else { // on a trouve la nbondary de fin de fichier mais il ya d'autres fichiers
        flushBuffer(0,boundaryPos - 2); // on veut tout mettre dans le fichier jusqu'a boundaryPos - 2 (\r\n)
        eraseInBody(0, _lenBody - boundaryPos - 2);
        _uploadFile.close();
        _fileState = ON_HEADER;
        return (CONTINUE);
    }
}

int    RawBits::checkBondaries( void  ) {

    if (_boundary.empty()) {        /* no bondary */
        // TODO: ?
        std::cerr << "ERROR: no bondary" << std::endl;
        return STOP;
    }

    // std::cerr << "------CHECK_BONDARY------\n" << _body << std::endl;
    while (true) {
        if (_fileState == ON_HEADER) {
            if (handleFileHeader() == STOP)
                return (NOT_FINISHED);
        } if (_fileState == ON_BODY) {
            int state = handleFileBody();

            if (state == NOT_FINISHED) {
                return (NOT_FINISHED);
            } else if (state == FINISHED) {
                return (FINISHED);
            } else if (state == CONTINUE) {
                continue ;
            }
        }
    }
    return (-1);
}

const std::vector<File*>& RawBits::getRawFile(void) const {
  return _files;
}
