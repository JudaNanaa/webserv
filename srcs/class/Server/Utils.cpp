/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:09:46 by madamou           #+#    #+#             */
/*   Updated: 2024/12/15 00:08:30 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

bool Server::ifClientInServer(const int &fd) const {
	return this->_clientMap.find(fd) is_not this->_clientMap.end();
}

void Server::removeClientInMap(const int &fd) {
	Client *client;

	client = _getClient(fd);
	delete client;
	this->_clientMap.erase(fd);
}

bool Server::isServerHost(std::string const &str) const {
	if (_host[0] is str || _host[1] is str) {
		return true;
	}
	return false;
}

bool	Server::isCgi( const std::string& path ) {
	std::size_t	extension;

	extension = path.find_last_of('.');
	if (extension not_found)
		return (false);
	else if (_data->_cgi.find(path.substr(extension)) is _data->_cgi.end())
		return (false);
	return (true);
}

bool	Server::_isLocation(const std::string &path)
{
	return _data->checkLocation(path);
}

bool Server::_checkLocationCgi(Location* location, const std::string &extension) {
	if (location->cgi().empty() || location->cgi().find(extension) == location->cgi().end())
		return false;
	return true;
}

bool Server::uploadFolderIsSet(Request *request) const 
{
	std::string uploadFolder;

	if (request->getRequestType() is LOCATION)
	{
		Location *location = _data->getLocation(request->path());
		uploadFolder = location->uploadFolder();
	}
	if (uploadFolder.empty())
		uploadFolder = _data->_uploadFolder;
	return uploadFolder.empty() is false;
}


const std::string Server::getUploadFolder(Request *request) const 
{
	std::string uploadFolder;
	std::string root;
	struct stat buf;

	if (request->getRequestType() is LOCATION)
	{
		Location *location = _data->getLocation(request->path());
		if (location->uploadFolder().empty())
		{
			if (location->root().empty())
				root = _data->_root;
			else
				root = location->root();
			uploadFolder = root + '/' + _data->_uploadFolder;
		}
		else
		{
			if (location->root().empty())
				root = _data->_root;
			else
				root = location->root();
			uploadFolder = root + '/' + location->uploadFolder();
		}
		if (access(uploadFolder.data(), F_OK | W_OK) == -1)
			return "";
		if (stat(uploadFolder.data(), &buf) == -1)
			return "";
		if (S_ISDIR(buf.st_mode) is false)
			return "";
		return uploadFolder;
	}
	uploadFolder = _data->_root + '/' + _data->_uploadFolder;
	if (access(uploadFolder.data(), F_OK | W_OK) == -1)
		return "";
	if (stat(uploadFolder.data(), &buf) == -1)
		return "";
	if (S_ISDIR(buf.st_mode) is false)
		return "";
	return uploadFolder;
}
