/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:46:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/12/06 20:53:51 by madamou          ###   ########.fr       */
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

std::string	File::get( std::string key ) {
	try {
		return (_infos.at(key));
	} catch (std::exception &e) {
		throw std::invalid_argument("not found: " + key);
	}
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

void	File::clean(void)
{
	_lenFile = 0;
	_infos.clear();
}


