/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingHeader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:51:03 by madamou           #+#    #+#             */
/*   Updated: 2024/12/12 17:19:36 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::_parseFirstLineHeader(Client *client, const std::vector<std::string> &headerSplit)
{
	Request *clientRequest = client->getRequest();
	std::string header = clientRequest->getHeader();
	std::vector<std::string> lineSplit;

	if (std::count(headerSplit[0].begin(), headerSplit[0].end(), ' ') is_not 2) {
		client->setResponse("400");
		throw std::invalid_argument("Error header 1: " + headerSplit[0]);
	}
	lineSplit = split(headerSplit[0], " ");
	if (lineSplit.size() != 3) {
		client->setResponse("400");
		throw std::invalid_argument("Error header 2: " + headerSplit[0]);
	}
	clientRequest->setMethode(lineSplit[0]);
	clientRequest->path(lineSplit[1]);
	if (lineSplit[2].compare(0, 6, "HTTP/1") != 0) {
		client->setResponse("505");
		throw std::invalid_argument("Error header 3: " + headerSplit[0]);
	}
}

void	Server::_addHeaderLine( std::string line, Request *clientRequest) {

	if (line.find(": ") not_found)
		throw std::invalid_argument("invalid line: " + line);

	std::string	value = line.substr(line.find(": ") + 2);
	std::string key = line.erase(line.find(": "));	// erase the value (keep the key)
	clientRequest->addHeaderLineToMap(key, value);
}

void Server::_parseOtherLinesHeader(Client *client, const std::vector<std::string> &headerSplit)
{
	Request *clientRequest = client->getRequest();

	std::vector<std::string>::const_iterator it;
	for (it = headerSplit.begin() + 1; it != headerSplit.end(); it++) {
		try {
			_addHeaderLine(*it, clientRequest);
		} catch (std::exception &e) {
			client->setResponse("400");
			throw std::invalid_argument(e.what());
		}
	}
}

void Server::_parseContentLengthAndBoundary(Request *clientRequest)
{
	if (clientRequest->isKeyfindInHeader("Content-Length") is true) {
		clientRequest->setSizeBody(std::atoll(clientRequest->find("Content-Length").c_str()));
		if (clientRequest->isKeyfindInHeader("Content-Type")) {
			if (clientRequest->find("Content-Type").find("boundary") is_found) {
				std::string bondary = clientRequest->find("Content-Type").substr(clientRequest->find("Content-Type").find("boundary") + 9);
				clientRequest->setBondary(bondary.c_str());
			}
		}
	}
}

void	Server::_handleLocation(Client *client) {
  	Request* request = client->getRequest();
  	Location* location = _data->getLocation(request->path());
	std::size_t	extension;

	std::cerr << "LOCATION" << std::endl;
	request->setRequestType(LOCATION);
	request->setLocation(location);
	extension = request->path().find_last_of('.');
	printnl("debug requests path : " << request->path());
	if (extension != std::string::npos) {
		// if (!_checkLocationCgi(location, request->path().substr(extension), client)) // je pense que c'est pas la bonne condition
		if (_checkLocationCgi(location, request->path().substr(extension), client) is true)
			return ;
	}

	if (!location->redirect().empty())
		request->setRedirect(true);

	if ((location->allowedMethods() & request->method()) is 0) {
		client->setResponse("405");
		return ;
	}

	long long client_max_body_size  = location->maxBodySize() < 0 ? _data->_clientMaxBodySize : location->maxBodySize();

	if (client_max_body_size >= 0) {
		if (client_max_body_size < request->getContentLenght()) {
    		client->setResponse("413");
    		return ;
		}
	}
	if (request->getContentLenght() is -1) // no body
    	client->setResponse();
}

void	Server::_handleRequest( Client *client ) {
	Request *request = client->getRequest();

	std::cerr << "DEFAULT" << std::endl;
	request->setRequestType(DEFAULT);
	if (isCgi(request->path()) is true) {
		_handleCGI(client);
		return;
	}

	if ((_data->_allowedMethods & request->method()) is 0) {
		client->setResponse("405");
		return ;
	}
	//check de la body Size
	if (_data->_clientMaxBodySize >= 0) {
		if (_data->_clientMaxBodySize < request->getContentLenght()) {
			client->setResponse("413");
			return ;
		}
	}
	if (request->getContentLenght() is -1)
		client->setResponse();
}

void	Server::_chooseParsing( Client *client ) {
	Request	*request = client->getRequest();
	
	if (_isLocation(request->path()) is true)
		_handleLocation(client);
	else
		_handleRequest(client);
}

void Server::_parseClientHeader(Client *client) {
	Request *clientRequest = client->getRequest();
	std::string header = clientRequest->getHeader();
	std::vector<std::string> headerSplit;
	std::vector<std::string> lineSplit;

	headerSplit = split(header, "\r\n");
	_parseFirstLineHeader(client, headerSplit); // Parse la 1ere ligne du header
	_parseOtherLinesHeader(client, headerSplit); // Parse les autres lignes

	std::cerr << *clientRequest << std::endl; // Print Request

	_parseContentLengthAndBoundary(clientRequest); // set content length et boundary
	clientRequest->setState(ON_BODY);
  	_chooseParsing(client); // apre avoir recuperer les infos, on choisie le parsing approprier grace aux informations recuperer
}

void Server::_addingHeader(Client *client, const char *buff, const int &n)
{
	try {
		client->pushHeaderRequest(buff, n);
	} catch (std::exception &e) {
		client->setResponse("505");
		throw std::runtime_error(e.what());
	}
	client->setUseBuffer(false);
	if (client->whatToDo() is READY_PARSE_HEADER)
		_parseClientHeader(client);
}
