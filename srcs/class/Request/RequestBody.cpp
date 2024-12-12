/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 00:01:58 by madamou           #+#    #+#             */
/*   Updated: 2024/12/12 06:46:09 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../Client/Client.hpp"

void	Request::_uploadBody(void) {
	bool	multipart;

	try {
		multipart = find("Content-Type").find("multipart") is_found;
	} catch (...) {
		multipart = false;
	}
	if (multipart is true) {
		uploadMultipart();
	} else {	// if no bondaries
		if (_defaultFile.is_open() is false) {
			_defaultFile.open(DEFAULT_UPLOAD_FILE, std::ios::trunc | std::ios::out);
			if (_defaultFile.fail())
				throw std::invalid_argument("failed to open DEFAULT_UPLOAD_FILE");
		}
		_defaultFile.write(getBody(), getLenBody());
		eraseInBody(0, getLenBody());
	}
}

void Request::addBodyRequest(const char *buff, const int &n, const bool &add) {
	if (add)
		_appendBody(buff, n);
	if (_method is POST_)
		_uploadBody();
	if (getLenTotalBody() is _contentLenght) {
		if (_defaultFile.is_open())
			_defaultFile.close();
		_client->setResponse();
	}
	else if (getLenTotalBody() > _contentLenght) {
		std::cerr << "LEN TOO LARGE: body: " << getLenTotalBody() << " | content length: " << _contentLenght << std::endl;
		std::cerr << "diff: " << getLenTotalBody() - _contentLenght << std::endl;
		_client->setResponse("413");
	}
}
