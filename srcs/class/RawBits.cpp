/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:57:06 by itahri            #+#    #+#             */
/*   Updated: 2024/11/05 14:57:14 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/RawBits.hpp"
#include <cstring>

RawBits::RawBits(void) {
	_content = NULL;
	_len = 0;
}

RawBits::~RawBits(void) {
	if (_content)
		delete[] _content;
}

void RawBits::pushBack(unsigned char c) {
	char *dest = new char[_len + 1];
	if (_content)
		std::memmove(dest, _content, _len);
	dest[_len] = c;
	delete [] _content;
	_content = dest;
	_len++;
}