/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 00:01:58 by madamou           #+#    #+#             */
/*   Updated: 2024/12/15 00:12:39 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"
#include <climits>
#include <unistd.h>

void	Request::_uploadBody(void) {
	bool	multipart;

	if (_uploadFolder.empty())
	{
		_uploadFolder = _server->getUploadFolder(this);
		printnl("uploadFolder == " << _uploadFolder);
		if (_uploadFolder.empty() is true)
		{
			setResponsCode("500");
			return;
		}
	}
	try {
		multipart = find("Content-Type").find("multipart") is_found;
	} catch (...) {
		multipart = false;
	}
	if (multipart is true)
		uploadMultipart();
	else {	// if no bondaries
		if (_defaultFile.is_open() is false) {
			_defaultFile.open( (_uploadFolder + '/' + DEFAULT_UPLOAD_FILE).c_str(), std::ios::trunc | std::ios::out);
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
	if (_method is POST_ && _server->uploadFolderIsSet(this))
		_uploadBody();
	else
	 	_client->setResponse();
	printnl("getLenTotalBody()  == " << getLenTotalBody() << "_contentLenght == " << _contentLenght);
	if (getLenTotalBody() is _contentLenght) {
		if (_defaultFile.is_open())
			_defaultFile.close();
		_client->setResponse();
	}
	else if (getLenTotalBody() > _contentLenght) {
		std::cerr << "LEN TOO LARGE: body: " << getLenTotalBody() << " | content length: " << _contentLenght << std::endl;
		std::cerr << "diff: " << getLenTotalBody() - _contentLenght << std::endl;
		_client->setResponse("413");
		if (_defaultFile.is_open())
		{
			_defaultFile.close();
			if (unlink(DEFAULT_UPLOAD_FILE) == -1)
				_client->setResponse("500");
		}
		else
		{
			if (_uploadFile.is_open())
				_uploadFile.close();
			if (unlink(_currentFile.get("filename").c_str()) == -1)
				_client->setResponse("500");
		}
	}
}
