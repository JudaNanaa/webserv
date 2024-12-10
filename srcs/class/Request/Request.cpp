/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:15:03 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/09 20:07:241 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp" 
#include "../../../includes/utils.hpp" 
#include "../Server/Server.hpp"
#include <map>
#include <fstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

Request::Request(Client *client, Server *server)
    : RawBits(),
      _state(ON_HEADER),
      _method(0),
      _ResponsCode("200"),
      _contentLenght(-1),
	  _server(server),
      _client(client),
      _isRedirect(false)
{

}

Request::~Request()
{

}

void	Request::openResponseFile(const char *fileName) {
	_responseFile.file.open(fileName);
	if (_responseFile.file.is_open() is true)
	{
		_responseFile.file.seekg(0, std::ios::end);
		_responseFile.fileSize = _responseFile.file.tellg();
		_responseFile.totalSend = 0;
		_responseFile.file.seekg(0);
	}
}

std::size_t	Request::readResponseFile(char *buffer, std::size_t n) {
	_responseFile.file.read(buffer, n);
	return _responseFile.file.gcount();
}


void	Request::closeResponseFile(void) {
	_responseFile.file.close();
}

void Request::addHeaderLineToMap(const std::string &key, const std::string &value) {
	if (key is "HOST") {
		if (_server->isServerHost(value) is false) // check si le host est bien celui du server
			throw std::invalid_argument("Error header: Not the server host: " + value);
	}
	if (_others.find(key) != _others.end())	// key already seen
		throw std::invalid_argument("duplicate argument: " + key);
	_others[key] = value;
}

bool Request::isKeyfindInHeader(std::string const &key) const {
	return _others.find(key) != _others.end();
}

void Request::addHeaderRequest(const char *buff, const int &n)
{
	RawBits::BuffToRaw(buff, n);
	if (RawBits::find("\r\n\r\n") is -1)
		return ;
	RawBits::splitRequest();
	_state = READY_PARSE_HEADER;
}

void	Request::_uploadBody(void) {
	bool	multipart;

	try {
		multipart = find("Content-Type").find("multipart") is_found;
	} catch (...) {
		multipart = false;
	}
	if (multipart is true) {
		RawBits::uploadMultipart();
	} else {	// if no bondaries
		if (_defaultFile.is_open() is false) {
			_defaultFile.open(DEFAULT_UPLOAD_FILE, std::ios::trunc | std::ios::out);
			if (_defaultFile.fail())
				throw std::invalid_argument("failed to open DEFAULT_UPLOAD_FILE");
		}
		_defaultFile.write(RawBits::getBody(), RawBits::getLenBody());
		eraseInBody(0, RawBits::getLenBody());
	}
}

void Request::addBodyRequest(const char *buff, const int &n, const bool &add) {
	if (add)
		appendBody(buff, n);
	if (_method is POST_)
		_uploadBody();
	if (RawBits::getLenTotalBody() is _contentLenght) {
		if (_defaultFile.is_open())
			_defaultFile.close();
		_client->setResponse();
	}	
	else if (RawBits::getLenTotalBody() > _contentLenght && _contentLenght != -1) {
		std::cerr << "LEN TOO LARGE: body: " << getLenTotalBody() << " | content length: " << _contentLenght << std::endl;
		std::cerr << "diff: " << getLenTotalBody() - _contentLenght << std::endl;
		_client->setResponse("413");
	}
}

std::ostream& operator<<(std::ostream& os, const Request& request ) {
	os
	<< "Request {\n"
	<< "\t" << "method: ";
	if (request.method() & GET_) {
		os << "GET";
	} else if (request.method() & POST_) {
		os << "POST";
	} else if (request.method() & DELETE_) {
		os << "DELETE";
	} else if (request.method() & OPTIONS_) {
		os << "OPTIONS";
	}  else {
		os << "none";
	} os << std::endl;

	os
	<< "\t" << "path: " << request.path() << std::endl
	<< std::endl;

	std::map<std::string, std::string>::const_iterator ite = request._others.end();
	for (std::map<std::string, std::string>::const_iterator it = request._others.begin(); it != ite; it++) {
		os << "\t" << it->first << ": " << it->second << std::endl;
	}

	os << "}" << std::endl;

	return os;
}
