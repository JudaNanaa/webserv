/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:00:53 by ibaby             #+#    #+#             */
/*   Updated: 2024/12/10 23:30:24 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Parser.hpp"

Location::Location() {
	_allowedMethods = 0;
	_client_max_body_size = -1; // no limit (?)

	_autoIndex = -1;

}

Location::~Location() {
}

/*	GETTER	*/

const std::string&	Location::location( void ) const {
	return _location;
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

const std::string&	Location::index( void ) const {
	return _index;
} // string
const int &Location::autoIndex( void ) const {
	return _autoIndex;
} // autoIndex

const std::string&	Location::uploadFolder( void ) const {
	return _uploadFolder;
} // string

/*	SETTER	*/

void	Location::location( std::string newLocation ) {
	_location = newLocation;
} // location

void	Location::root( std::string newRoot ) {
	_root = newRoot;
} // root

void	Location::redirect( std::string newRedirect ) {
	_redirect = newRedirect;
} // redirect
void	Location::allowedMethods( int newAllowedMethods ) {
	_allowedMethods = newAllowedMethods;
} // allowedMethods
void	Location::maxBodySize( long long newMaxBodySize ) {
	_client_max_body_size = newMaxBodySize;
} // maxBodySize

void	Location::index( std::string newIndex ) {
	_index = newIndex;
} // index
void	Location::autoIndex( const int &newAutoIndex ) {
	_autoIndex = newAutoIndex;
} // autoIndex

void	Location::uploadFolder( std::string newUploadFolder ) {
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

void Location::addCgi(std::string cgi) {
  //TODO: add cgi logic
  if (cgi.find(" ") is_found) {
    _cgi[cgi.substr(0, cgi.find(' '))] = trim(cgi.substr(cgi.find(' ')));
  }
}