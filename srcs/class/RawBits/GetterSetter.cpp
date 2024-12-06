/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetterSetter.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:41:17 by madamou           #+#    #+#             */
/*   Updated: 2024/12/06 19:00:50 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawBits.hpp"
#include <string>

void RawBits::setBondary(const std::string &str) {
	_boundary = str;
}

const std::string &RawBits::getBondary(void) const {
	return _boundary;
}

const std::string &RawBits::getHeader(void) const {
	return _header;
} 

const char *RawBits::getBody(void) const {
	return _body;
}

const unsigned int &RawBits::getLenBody(void) const {
	return _lenBody;
}

const long long &RawBits::getLenTotalBody(void) const {
	return _lenTotalBody;
}
