/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:46:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/29 19:40:00 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <stdexcept>

File::File() {
}

File::~File() {
	
}

void	File::freeContent(void) {
	
}

const char*	File::content( void ) {
	return (_content);	
}

std::string	File::get( std::string key ) {
	try {
		return (_infos.at(key));
	} catch (std::exception &e) {
		throw std::invalid_argument("not found: " + key);
	}
}

void	File::content( char* newContent ) {
	_content = newContent;
}

void	File::set( std::string key, std::string value ) {
	_infos[key] = value;
}

void	File::lenFile(size_t len) {
	_lenFile = len;
}

const size_t &File::lenFile(void) const {
	return _lenFile;
}