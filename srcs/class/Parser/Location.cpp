/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:00:53 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/09 19:33:33 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Location.hpp"

Location::Location() {
	_location = "";

	_root = "";
	_cgi = "";
	_redirect = "";
	_allowedMethods = 0;
	_client_max_body_size = -1; // no limit (?)

	_index = "";
	_autoIndex = false;

	_file_upload = false;
	_uploadFolder = "";
}

Location::~Location() {
}

/*; 	copy needed ?	*/
// Location::Location( const Location &other ) {
// 	(*this) = other;
// }

// Location& Location::operator=( const Location &other ) {
// 	_root = other._root;
// 	_cgi = other._cgi;
// 	_redirect = other._redirect;
// 	_allowedMethods = other._allowedMethods;
// 	_client_max_body_size = other._client_max_body_size;

// 	_index = other._index;
// 	_autoIndex = other._autoIndex;

// 	_file_upload = other._file_upload;
// 	_uploadFolder = other._uploadFolder;
//	return *this;
// }

/*	GETTER	*/

const std::string&	Location::location( void ) const {
	return _location;
} // LOCATION

const std::string&	Location::root( void ) const {
	return _root;
} // string
const std::string&	Location::cgi( void ) const {
	return _cgi;
} // string
const std::string&	Location::redirect( void ) const {
	return _redirect;
} // string
int Location::allowedMethods( void ) const {
	return _allowedMethods;
} // allowedMethods
int	Location::maxBodySize( void ) const {
	return _client_max_body_size;
} // maxBodySize

const std::string&	Location::index( void ) const {
	return _index;
} // string
bool	Location::autoIndex( void ) const {
	return _autoIndex;
} // autoIndex

bool	Location::fileUpload( void ) const {
	return _file_upload;
} // fileUpload
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
void	Location::cgi( std::string newCgi ) {
	_cgi = newCgi;
} // cgi
void	Location::redirect( std::string newRedirect ) {
	_redirect = newRedirect;
} // redirect
void	Location::allowedMethods( int newAllowedMethods ) {
	_allowedMethods = newAllowedMethods;
} // allowedMethods
void	Location::maxBodySize( int newMaxBodySize ) {
	_client_max_body_size = newMaxBodySize;
} // maxBodySize

void	Location::index( std::string newIndex ) {
	_index = newIndex;
} // index
void	Location::autoIndex( bool newAutoIndex ) {
	_autoIndex = newAutoIndex;
} // autoIndex

void	Location::fileUpload( bool newFileUpload ) {
	_file_upload = newFileUpload;
} // fileUpload
void	Location::uploadFolder( std::string newUploadFolder ) {
	_uploadFolder = newUploadFolder;
} // uploadFolder

std::string	isDefault(const std::string& var) {
	return (var.empty()? "default":var);
}

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

std::ostream& operator<<(std::ostream& os, const Location& location) {
	std::cout << "location " << isDefault(location.location()) << "\n{" << std::endl;
	std::cout << "\t" << "root: " << isDefault(location.root()) << std::endl;
	std::cout << "\t" << "index: " << isDefault(location.index()) << std::endl;
	std::cout << "\t" << "autoIndex: " << (location.autoIndex()? "true":"false") << std::endl;
	std::cout << "\t" << "cgi: " << isDefault(location.cgi()) << std::endl;
	std::cout << "\t" << "redirect: " << isDefault(location.redirect()) << std::endl;
	std::cout << "\t" << "allowed Methods: " << printMethods(location.allowedMethods()) << std::endl;
	if (location.maxBodySize() == -1)
		std::cout << "\t" << "max body size: " << "default" << std::endl;
	else
		std::cout << "\t" << "max body size: " << location.maxBodySize() << std::endl;
	std::cout << "\t" << "file upload: " << (location.fileUpload()? "true":"false") << std::endl;
	std::cout << "\t" << "upload folder: " << isDefault(location.uploadFolder()) << std::endl;
	std::cout << "}" << std::endl;
	
	return (os);
}
