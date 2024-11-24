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

Request::Request(Client *client) : RawBits() {
	_method = 0;
	_state = ON_HEADER;
	_contentLenght = -1;
	_ResponsCode = "200";
	_isRedirect = false;
	_client = client;
}

Request::~Request() {
	
}

void Request::addRequestToMap(std::string key, std::string value) {
	if (key == "HOST") {
		if (_server->isServerHost(value) == false) { // check si le host est bien celui du server
			throw std::invalid_argument("Error header: Not the server host: " + value);
		}
	}
	if (_others.find(key) != _others.end()) {	// key already seen
		throw std::invalid_argument("duplicate argument: " + key);
	}
	_others[key] = value;
}

bool Request::isKeyfindInHeader(std::string const &key) const {
	return _others.find(key) != _others.end();
}

t_parse	Request::addHeaderRequest(char *buff, int n) {
	RawBits::BuffToRaw(buff, n);
	if (RawBits::find("\r\n\r\n") == -1) {
		return NOT_READY;
	}
	RawBits::splitRequest();

	return READY_PARSE_HEADER;
}

void	Request::uploadBody() {
	std::cerr << "es ce que je passe par la ?" << std::endl;
	if (find("Content-Type").find("multipart") != std::string::npos) {
		// find bondaries
		if (RawBits::checkBondaries() == FINISHED) {	// transform this function
			_client->setReadyToresponse(true);
		}
		std::cerr << "\e" << getCurrentFile()->get("filename") << ": " << (RawBits::getLenTotalBody() / _contentLenght) * 100 << "%\r";
	} else {	// if no bondaries (?)
		std::fstream	file(DEFAULT_UPLOAD_FILE);
		if (file.fail())
			throw std::invalid_argument("failed to open DEFAULT_UPLOAD_FILE");

		file.write(RawBits::getBody(), RawBits::getLenBody());
	}
}

t_parse	Request::addBodyRequest(char *buff, int n, bool add) {
	if (add)
		appendBody(buff, n);
	uploadBody();
	if (RawBits::getLenTotalBody() > _contentLenght) {
		std::cerr << "LEN TOO LARGE: body: " << getLenTotalBody() << " | content length: " << _contentLenght << std::endl;
		std::cerr << "diff: " << getLenTotalBody() - _contentLenght << std::endl;
   		setResponsCode("400");
		return ERROR;
	}
	return NOT_READY;
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
	<< "\t" << "host: " << request.host()
	<< std::endl;

	std::map<std::string, std::string>::const_iterator ite = request._others.end();
	for (std::map<std::string, std::string>::const_iterator it = request._others.begin(); it != ite; it++) {
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	}

	os << "}" << std::endl;

	return os;
}
