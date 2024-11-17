/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:46:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/17 22:33:39 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"
#include <cstddef>
#include <cstring>
#include <exception>
#include <stdexcept>

File::File() {
}

File::~File() {
}

// File( const File& other );
// File& operator=( const File& other );

const char*	File::content( void ) {
	return (_content);	
}

const std::string&	File::contentDisposition( void ) {
	return (_contentDisposition);
}

const std::string&	File::name( void ) {
	return (_name);
}

const std::string&	File::filename( void ) {
	return (_filename);	
}

const std::string&	File::contentType( void ) {
	return (_contentType);	
}

std::string	File::get( std::string key ) {
	try {
		// return (_others.at(key));
		return (_infos.at(key));
	} catch (std::exception &e) {
		throw std::invalid_argument("not found: " + key);
	}
}

void	File::content( char* newContent ) {
	_content = newContent;
}

void	File::contentDisposition( std::string newContentDisposition ) {
	_contentDisposition = newContentDisposition;
}

void	File::name( std::string newName ) {
	_name = newName;

	if (_name.find(".") == std::string::npos)		/*	no extension	*/
		return ;

	_extension = _name.substr(_name.find_first_of('.'));
}

void	File::filename( std::string newFilename ) {
	_filename = newFilename;
}

void	File::contentType( std::string newContentType ) {
	_contentType = newContentType;
}

void	File::set( std::string key, std::string value ) {
	_others[key] = value;
	_infos[key] = value;
}

