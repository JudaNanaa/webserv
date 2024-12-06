/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingHeader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 01:51:03 by madamou           #+#    #+#             */
/*   Updated: 2024/12/06 16:43:06 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::_parseFirstLineHeader(Client *client, const std::vector<std::string> &headerSplit)
{
	Request *clientRequest = client->getRequest();
	std::string header = clientRequest->getHeader();
	std::vector<std::string> lineSplit;

	if (std::count(headerSplit[0].begin(), headerSplit[0].end(), ' ') is_not 2) {
		// La premiere ligne est pas bonne donc faire une reponse en fonction
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

void	Server::chooseParsing( Client *client ) {
	Request	*request = client->getRequest();

	if (_data->checkLocation(request->path()) != NULL) {
		request->setRequestType(LOCATION);
		std::cerr << "LOCATION" << std::endl;
		handleLocation(client);
	} else if (isCgi(request->path()) is true) {
		request->setRequestType(CGI);
		std::cerr << "CGI" << std::endl;
		handleCgi(client);
	} else {
		request->setRequestType(DEFAULT);
		std::cerr << "DEFAULT" << std::endl;
		handleRequest(client);
	}
	request->setStatus(ON_BODY);
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
  	chooseParsing(client); // apre avoir recuperer les infos, on choisie le parsing approprier grace aux informations recuperer
}

void Server::_addingHeader(Client *client, char *buff, int n)
{
	try {
		client->pushHeaderRequest(buff, n);
	} catch (std::exception &e) {
		client->setResponse("505");
		throw std::runtime_error(e.what());
	}
	client->setUseBuffer(false);
	if (client->getReadyToParseHeader())
		_parseClientHeader(client);
}
