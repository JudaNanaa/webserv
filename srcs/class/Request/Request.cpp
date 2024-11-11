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

#include "../../../includes/Request.hpp" 
#include "../../../includes/utils.hpp" 
#include "../../../includes/Server.hpp"
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

Request::Request() {
	_method = 0;
	_path = "";
	_Host = "";
}

Request::~Request() {
}

int	Request::method( void	) const {
	return (_method);
}

const std::string&	Request::path( void	) const {
	return (_path);
}

const std::string&	Request::host( void	) const {
	return (_Host);
}

const std::string&	Request::find( std::string key ) const {
	if (_others.find(key) == _others.end())
		throw std::invalid_argument(key + "key not found");

	return (_others.at(key));
}

void	Request::method( int newMethod ) {
	_method = newMethod;
}

void	Request::host( std::string newHost ) {
	_Host = newHost;
}

void	Request::path( std::string newPath ) {
	_path = newPath;
}

void	Request::addRequest(std::string str) {
	_request += str;
	if (_request.find("\r\n\r\n") == std::string::npos) {
		return ;
	}
	std::cout << "Adding request..." << std::endl;
	parseRequest();
}

void	Request::parseRequestLine( std::string line ) {

	if (line.find(": ") == std::string::npos)
		throw std::invalid_argument("invalid line: " + line);

	std::string	value = line.substr(line.find(": "));

	std::string key = line.erase(line.find(": "));	// erase the value (keep the key)

	if (line == "HOST") {
		throw std::invalid_argument("duplicate argument: HOST");
	} else {
		if (_others.find(key) != _others.end()) {	// key already seen
			throw std::invalid_argument("duplicate argument: " + key);
		} else {
			_others[key] = value;
		}
	}

}

void	Request::parseRequest(void) {
	std::string header;
	std::vector<std::string> headerSplit;
	std::vector<std::string> lineSplit;

	std::cout << "Request incoming..." << std::endl;
	header = _request.substr(0, _request.find("\r\n\r\n"));
	headerSplit = split(header, "\r\n");

	std::cout << "DEBUG HEADER: \n" << header << std::endl;

	if (std::count(headerSplit[0].begin(), headerSplit[0].end(), ' ') != 2) {
		// La premiere ligne est pas bonne donc faire une reponse en fonction
		std::cout << "Error parseRequest 1" << std::endl;
		return;
	}

	lineSplit = split(headerSplit[0], " ");
	if (lineSplit.size() != 3) { // not always 3 part
		// La premiere ligne est pas bonne donc faire une reponse en fonction
		std::cout << "Error parseRequest 2" << std::endl;
		return;
	}

	if (!_server->checkAllowMethodes(lineSplit[0]))
    	std::cout << "Error Invalid Method : [" + lineSplit[0] + "]" << std::endl; // need correct gesture

	this->_path = lineSplit[1];
	if (lineSplit[2].compare("HTTP/1.1") != 0) {
		// le htpp nest pas bon !!
		std::cout << "Error parseRequest 3" << std::endl;
		return ;
	}

	lineSplit = split(headerSplit[1], ": "); // check line host
	if (lineSplit.size() != 2) {
		std::cout << "Error parseRequest 4" << std::endl;
		return;
	}
	if (lineSplit[0].compare("Host") != 0) {
		std::cout << "Error parseRequest2" << std::endl;
		return;	
	}
	this->_Host = lineSplit[1];
	if (_server->isServerHost(this->_Host) == false) { // check si le host est bien celui du server
		std::cout << "Error parseRequest1" << std::endl;
		return;
	}

	for (std::vector<std::string>::const_iterator it = headerSplit.begin() + 2, ite = headerSplit.end();
			it != ite; it++) {
		parseRequestLine(*it);
	}

	std::cout << "REQUEST:\n" << *this << std::endl;

}

std::ostream& operator<<(std::ostream& os, const Request& request ) {
	os
	<< "Request {\n"
	<< "\t" << "method: ";
	if (request.method() | GET_) {
		os << "GET";
	} else if (request.method() | POST_) {
		os << "POST";
	} else if (request.method() | DELETE_) {
		os << "DELETE";
	} else if (request.method() | OPTIONS_) {
		os << "OPTIONS";
	}  else {
		os << "none";
	} os << std::endl;

	os
	<< "\t" << "path: " << request.path()
	<< "\t" << "host: " << request.host()
	<< std::endl;

	std::map<std::string, std::string>::const_iterator ite = request._others.end();
	for (std::map<std::string, std::string>::const_iterator it = request._others.begin(); it != ite; it++) {
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	}

	os << "}" << std::endl;

	return os;
}

void	Request::addServer(Server* server) {
  _server = server;
} 
