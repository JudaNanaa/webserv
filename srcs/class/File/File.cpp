/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:46:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/15 19:20:50 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"
#include <exception>
#include <memory_resource>
#include <stdexcept>

File::File() {
}

File::~File() {
}

// File( const File& other );
// File& operator=( const File& other );

const std::string&	File::content( void ) {
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
		return (_others.at(key));
	} catch (std::exception &e) {
		throw std::invalid_argument("not found: " + key);
	}
}

void	File::content( std::string newContent ) {
	_content = newContent;
}

void	File::contentDisposition( std::string newContentDisposition ) {
	_contentDisposition = newContentDisposition;
}

void	File::name( std::string newName ) {
	_name = newName;
}

void	File::filename( std::string newFilename ) {
	_filename = newFilename;
}

void	File::contentType( std::string newContentType ) {
	_contentType = newContentType;
}

void	File::set( std::string key, std::string value ) {
	_others[key] = value;
}

