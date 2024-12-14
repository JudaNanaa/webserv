/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseFile.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:26:59 by madamou           #+#    #+#             */
/*   Updated: 2024/12/14 16:27:51 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include <sys/stat.h>

std::string Server::_getFinalPath(Request *clientRequest)
{
	std::string finalPath;

	if (clientRequest->getRequestType() is LOCATION)
	{
		Location *location = clientRequest->getLocation();
		if (location->root().empty())
			finalPath = _data->_root + clientRequest->path();
		else 
			finalPath =  location->root() + &clientRequest->path()[location->location().size()];
	}
	else if (clientRequest->getRequestType() is DEFAULT)
		finalPath = _data->_root + clientRequest->path();
	formatSlash(finalPath);
	return finalPath;
}

std::string Server::_normalOpenFile(Request *clientRequest, Client* client)
{
	std::string finalPath;
	struct stat buf;

	finalPath = _getFinalPath(clientRequest);
	if (access(finalPath.data(), F_OK | R_OK) is -1)
		clientRequest->setResponsCode("404");
	else if (stat(finalPath.data(), &buf) is -1)
		clientRequest->setResponsCode("500");
	else if (S_ISDIR(buf.st_mode))
		finalPath = _manageIfDirectory(client, clientRequest, finalPath);
	else
		clientRequest->openResponseFile((finalPath).c_str());
	if (clientRequest->getState() == SEND)
		return finalPath;
	if (clientRequest->getResponsCode() is "200" && clientRequest->responseFileOpen() is false)
		clientRequest->setResponsCode("404");
	return finalPath;
}

std::string Server::_openResponseFile(Request *clientRequest, Client* client)
{
	std::string	finalPath;

	if (clientRequest->getResponsCode() is "200")
		finalPath = _normalOpenFile(clientRequest, client);
	if (clientRequest->getState() == SEND)
		return finalPath;
	if (clientRequest->getResponsCode() != "200")
	{
		finalPath = "URIs/errors/" + clientRequest->getResponsCode() + ".html";
		clientRequest->openResponseFile(finalPath.c_str());
	}
	return finalPath;
}
