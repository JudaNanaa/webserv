/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:00:53 by ibaby             #+#    #+#             */
/*   Updated: 2024/12/14 17:24:08 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Parser.hpp"
#include <vector>

Location::Location() {
	_allowedMethods = 0;
	_client_max_body_size = -1; // no limit (?)
	_autoIndex = -1;
}

Location::~Location() {}

/*	GETTER	*/

const std::string&	Location::location( void ) const {
	return _location;
} // LOCATION

const std::string&	Location::getErrorPage(const std::string &code) const {
	return _errorPages.at(code);
} // LOCATION

const std::string&	Location::root( void ) const {
	return _root;
} // string

const std::map<std::string, std::string>& Location::cgi( void ) const {
	return _cgi;
} // string
const std::string&	Location::redirect( void ) const {
	return _redirect;
} // string
const int &Location::allowedMethods( void ) const {
	return _allowedMethods;
} // allowedMethods
const long long	&Location::maxBodySize( void ) const {
	return _client_max_body_size;
} // maxBodySize

const std::vector<std::string>&	Location::index( void ) const {
	return _index;
} // string
const int &Location::autoIndex( void ) const {
	return _autoIndex;
} // autoIndex

const std::string&	Location::uploadFolder( void ) const {
	return _uploadFolder;
} // string

/*	SETTER	*/

void	Location::location( std::string &newLocation ) {
	_location = newLocation;
} // location

void	Location::root( std::string &newRoot ) {
	_root = newRoot;
} // root

void	Location::redirect( std::string &newRedirect ) {
	_redirect = newRedirect;
} // redirect
void	Location::allowedMethods( int newAllowedMethods ) {
	_allowedMethods = newAllowedMethods;
} // allowedMethods
void	Location::maxBodySize( long long newMaxBodySize ) {
	_client_max_body_size = newMaxBodySize;
} // maxBodySize

void	Location::index( std::string &newIndex ) {
	_index = split(newIndex, " ");
} // index
void	Location::autoIndex( const int &newAutoIndex ) {
	_autoIndex = newAutoIndex;
} // autoIndex

void	Location::uploadFolder( std::string &newUploadFolder ) {
	_uploadFolder = newUploadFolder;
} // uploadFolder

std::string	printMethods(int methods) {
	std::string	result;

	if (methods & GET_) {
		result += "GET ";
	} if (methods & POST_) {
		result += "POST ";
	} if (methods & DELETE_) {
		result += "DELETE ";
	} if (methods & OPTIONS_) {
		result += "OPTIONS ";
	} if (result.empty()) {
		result = "none";
	}
	return (result);
}

void Location::addCgi(std::string &cgi) {
  //TODO: add cgi logic
  if (cgi.find(" ") is_found) {
    _cgi[cgi.substr(0, cgi.find(' '))] = trim(cgi.substr(cgi.find(' ')));
  }
}

void Location::handleAutoIndex(std::string &value) {
    value == "on" ? autoIndex(true) : value == "off" ? autoIndex(false) :
      throw std::invalid_argument("invalid value: expected 'on' or 'off'");
}

void Location::handleMaxBodySize(std::string &value) {
    long long	int_value = std::atoll(value.c_str());
    if (int_value <= 0 || value.find_first_not_of("0123456789") is_found)
      throw std::invalid_argument("invalid value");
    maxBodySize(int_value);
}

void Location::handleAllowedMethods(std::string &value) {
  int methods = 0;

	if (value.find("GET") is_found)
		methods = methods | GET_;
	if (value.find("POST") is_found)
		methods = methods | POST_;
	if (value.find("DELETE") is_found)
		methods = methods | DELETE_;

	if (methods == 0)	// no methods found
		throw std::invalid_argument("GET, DELETE or POST expected");
	allowedMethods(methods);
}

bool Location::errorPageIsSet(const std::string &code) const
{
	return _errorPages.find(code) != _errorPages.end();
}

void	Location::setErrorPage(std::string &value)
{
	std::vector<std::string> vec;
	
	vec = split(value, " ");
	if (vec.size() != 2)
		throw std::invalid_argument("Invalid argument error_pages");
	if (allDigit(vec[0].data()) == false)
		throw std::invalid_argument("Not a number at error_pages");
    _errorPages[vec[0]] = vec[1];
}