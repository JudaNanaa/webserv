/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetterSetterRequest.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:31:09 by madamou           #+#    #+#             */
/*   Updated: 2024/12/12 17:20:13 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../Client/Client.hpp"
#include <cstddef>
#include <iterator>

void Request::setRequestType(const int &type) {
	_requestType = type;
}

const int &Request::getRequestType(void) const {
	return _requestType;
}

const int &Request::method( void	) const {
	return (_method);
}

const std::string&	Request::path( void	) const {
	return (_path);
}

const std::string&	Request::find( std::string key ) const {
	if (_others.find(key) == _others.end())
		throw std::invalid_argument(key + ": key not found");

	return (_others.at(key));
}

const t_state &Request::getState(void) const {
	return _state;
}

void	Request::method( const int &newMethod ) {
	_method = newMethod;
}

void	Request::setState( t_state newState ) {
	_state = newState;
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

void Request::setMethode(const std::string &methode) {
	if (methode == "GET")
		_method = GET_;
	else if (methode == "POST")
		_method = POST_;
	else if (methode == "DELETE")
		_method = DELETE_;
}

void Request::setResponsCode(const std::string &code) {
	_ResponsCode = code;
	_state = RESPONSE;
}

const std::string& Request::getResponsCode(void) const {
	return _ResponsCode;
}

void Request::setRedirect(const bool &b) {
	_isRedirect = b;
}

bool Request::getRedirect(void) const{
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

void	Request::setLocation(Location *location) {
	_location = location;
}

Location	*Request::getLocation(void) const {
	return _location;
}