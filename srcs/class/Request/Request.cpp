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
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

Request::Request(Client *client) : RawBits() {
	_method = 0;
	_state = ON_HEADER;
	_contentLenght = 0;
	_ResponsCode = "200";
	_isRedirect = false;
	_client = client;
}

Request::~Request() {
	
}

void Request::setIsACgi(bool boolean) {
	_isCgi = boolean;
}

bool Request::isACgi(void) const {
	return _isCgi;
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

const t_state &Request::getStatus(void) const {
	return _state;
}

void	Request::method( int newMethod ) {
	_method = newMethod;
}

void	Request::host( std::string newHost ) {
	_Host = newHost;
}

void	Request::setStatus( t_state newStatus ) {
	_state = newStatus;
}

void	Request::path( std::string newPath ) {
	_path = newPath;
}

void	Request::setSizeBody(unsigned int nb) {
	_contentLenght = nb;
}

const unsigned int	&Request::getContentLenght(void) const {
	return _contentLenght;
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
	_state = ON_BODY;
	return READY_PARSE_HEADER;
}

t_parse	Request::addBodyRequest(char *buff, int n, bool add) {
	if (add == true) {
		RawBits::appendBody(buff, n);
		RawBits::BuffToRaw(buff, n);
	}
	if (RawBits::getLenBody() == _contentLenght) {
		return READY_PARSE_BODY;
	}
	else if (RawBits::getLenBody() > _contentLenght) {
   		setResponsCode("400");
		return READY_PARSE_BODY;
	}
	return NOT_READY;
}

std::string& Request::getMap(std::string key) {
  static std::string def = "";
  if (_others.find("key") != _others.end())
    return _others[key];
  return def;
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

void	Request::addServer(Server* server) {
  _server = server;
} 

void Request::setMethode(std::string methode) {
	if (methode == "GET")
		_method = GET_;
	else if (methode == "POST")
		_method = POST_; 
	else if (methode == "DELETE")
		_method = DELETE_;
}

const int& Request::getMethode(void) const {
	return _method;
}

void Request::setResponsCode(std::string code) {
	_client->setReadyToresponse(true);
	_ResponsCode = code;
}

const std::string& Request::getResponsCode(void) const {
	return _ResponsCode;
}

const std::vector<File*>& Request::getFile(void) const{
  return RawBits::getRawFile();
}

void Request::setRedirect(bool b) {
	_isRedirect = b;
}

bool& Request::getRedirect(void) {
	return _isRedirect;
}