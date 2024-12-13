/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHeader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 00:00:58 by madamou           #+#    #+#             */
/*   Updated: 2024/12/13 10:15:26 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../Server/Server.hpp"

void Request::addHeaderLineToMap(const std::string &key, const std::string &value) {
	if (key is "HOST") {
		if (_server->isServerHost(value) is false) // check si le host est bien celui du server
			throw std::invalid_argument("Error header: Not the server host: " + value);
	}
	if (_others.find(key) != _others.end())	// key already seen
		throw std::invalid_argument("duplicate argument: " + key);
	_others[key] = value;
}

bool Request::isKeyfindInHeader(const std::string &key) const {
	return _others.find(key) != _others.end();
}

void Request::addHeaderRequest(const char *buff, const int &n)
{
	BuffToRequest(buff, n);
	if (_findInRequest("\r\n\r\n") is -1)
		return ;
	splitRequest();
	_state = READY_PARSE_HEADER;
}
