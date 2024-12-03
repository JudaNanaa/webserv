/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetterSetter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:31:09 by madamou           #+#    #+#             */
/*   Updated: 2024/12/03 20:07:59 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../Client/Client.hpp"
#include <cstddef>
#include <iterator>

void Request::setIsACgi(bool boolean) {
	_isCgi = boolean;
}

bool Request::isACgi(void) const {
	return _isCgi;
}

const int &Request::method( void	) const {
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
		throw std::invalid_argument(key + ": key not found");

	return (_others.at(key));
}

const t_state &Request::getStatus(void) const {
	return _state;
}

void	Request::method( const int &newMethod ) {
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

void	Request::setSizeBody(long long nb) {
	_contentLenght = nb;
}

const long	long &Request::getContentLenght(void) const {
	return _contentLenght;
}

std::string& Request::getMap(const std::string &key) {
  static std::string def = "";
  if (_others.find("key") != _others.end())
    return _others[key];
  return def;
}

void	Request::addServer(Server* server) {
  _server = server;
} 

void Request::setMethode(const std::string &methode) {
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

void Request::setResponsCode(const std::string &code) {
	_ResponsCode = code;
}

const std::string& Request::getResponsCode(void) const {
	return _ResponsCode;
}

void Request::setRedirect(const bool &b) {
	_isRedirect = b;
}

bool& Request::getRedirect(void) {
	return _isRedirect;
}

void	Request::incrementSizeBody(const unsigned long long &n)
{
	RawBits::_lenBody += n;
	RawBits::_lenTotalBody += n;
}

bool Request::responseFileOpen(void) const {
	return _responseFile.file.is_open();
}

std::size_t Request::getResponseFileSize(void) const {
	return _responseFile.fileSize;
}

std::size_t Request::getResponseFileTotalSend(void) const {
	return _responseFile.totalSend;
}
		
void	Request::addResponseFileTotalSend(std::size_t nbSend) {
	_responseFile.totalSend += nbSend;
}