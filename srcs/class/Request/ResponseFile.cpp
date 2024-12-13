/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFile.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 23:58:32 by madamou           #+#    #+#             */
/*   Updated: 2024/12/13 10:15:04 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::openResponseFile(const char *fileName) {
	_responseFile.file.open(fileName);
	if (_responseFile.file.is_open() is true)
	{
		_responseFile.file.seekg(0, std::ios::end);
		_responseFile.fileSize = _responseFile.file.tellg();
		_responseFile.totalSend = 0;
		_responseFile.file.seekg(0);
	}
}

std::size_t	Request::readResponseFile(char *buffer, const std::size_t &n) {
	_responseFile.file.read(buffer, n);
	return _responseFile.file.gcount();
}


void	Request::closeResponseFile(void) {
	_responseFile.file.close();
}
