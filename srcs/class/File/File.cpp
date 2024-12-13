/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:46:21 by ibaby             #+#    #+#             */
/*   Updated: 2024/12/13 09:59:53 by madamou          ###   ########.fr       */
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

const std::string	&File::get(const std::string &key) {
	try {
		return (_infos.at(key));
	} catch (std::exception &e) {
		throw std::invalid_argument("not found: " + key);
	}
}

void	File::set(const std::string &key, const std::string &value) {
	_infos[key] = value;
}

void	File::clean(void)
{
	_infos.clear();
}


