/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 01:01:30 by madamou           #+#    #+#             */
/*   Updated: 2024/12/03 22:07:58 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SECRET "https://www.cat29.fr/riche-et-independant/"
#define SINGE "https://media.tenor.com/_uIJwdpxI8UAAAAM/mono-serio.gif"
#define MYCEOO "https://pbs.twimg.com/media/EaoNasxXYAUE8pA.jpg"
#define MYCEOC "https://pbs.twimg.com/media/EaoNYXkXkAMHJJK.jpg"


const std::string getMessageCode(int code) {
	std::map<int, std::string> codes_responses;

	codes_responses[100] = "Continue";
	codes_responses[101] = "Switching Protocols";
	codes_responses[102] = "Processing";
	codes_responses[103] = "Early Hints";
	codes_responses[200] = "OK";
	codes_responses[201] = "Created";
	codes_responses[202] = "Accepted";
	codes_responses[203] = "Non-Authoritative Information";
	codes_responses[204] = "No Content";
	codes_responses[205] = "Reset Content";
	codes_responses[206] = "Partial Content";
	codes_responses[207] = "Multi-Status";
	codes_responses[208] = "Already Reported";
	codes_responses[210] = "Content Different";
	codes_responses[226] = "IM Used";
	codes_responses[300] = "Multiple Choices";
	codes_responses[301] = "Moved Permanently";
	codes_responses[302] = "Found";
	codes_responses[303] = "See Other";
	codes_responses[304] = "Not Modified";
	codes_responses[305] = "Use Proxy";
	codes_responses[307] = "Temporary Redirect";
	codes_responses[308] = "Permanent Redirect";
	codes_responses[310] = "Too many Redirects";
	codes_responses[400] = "Bad Request";
	codes_responses[401] = "Unauthorized";
	codes_responses[402] = "Payment Required";
	codes_responses[403] = "Forbidden";
	codes_responses[404] = "Not Found";
	codes_responses[405] = "Method Not Allowed";
	codes_responses[406] = "Not Acceptable";
	codes_responses[407] = "Proxy Authentication Required";
	codes_responses[408] = "Request Time-out";
	codes_responses[409] = "Conflict";
	codes_responses[410] = "Gone";
	codes_responses[411] = "Length Required";
	codes_responses[412] = "Precondition Failed";
	codes_responses[413] = "Request Entity Too Large";
	codes_responses[414] = "Request-URI Too Long";
	codes_responses[415] = "Unsupported Media Type";
	codes_responses[416] = "Requested range unsatisfiable";
	codes_responses[417] = "Expectation failed";
	codes_responses[418] = "I'm a teapot";
	codes_responses[419] = "Page expired";
	codes_responses[421] = "Bad mapping / Misdirected Request";
	codes_responses[422] = "Unprocessable entity";
	codes_responses[423] = "Locked";
	codes_responses[424] = "Method failure";
	codes_responses[425] = "Too Early";
	codes_responses[426] = "Upgrade Required";
	codes_responses[427] = "Invalid digital signature";
	codes_responses[428] = "Precondition Required";
	codes_responses[429] = "Too Many Requests";
	codes_responses[431] = "Request Header Fields Too Large";
	codes_responses[449] = "Retry With";
	codes_responses[450] = "Blocked by Windows Parental Controls";
	codes_responses[451] = "Unavailable For Legal Reasons";
	codes_responses[456] = "Unrecoverable Erstatus()";
	codes_responses[505] = "HTTP Version not supported";
	codes_responses[506] = "Variant Also Negotiates";
	codes_responses[507] = "Insufficient storage";
	codes_responses[508] = "Loop detected";
	codes_responses[509] = "Bandwidth Limit Exceeded";
	codes_responses[510] = "Not extended";
	codes_responses[511] = "Network authentication required";
	codes_responses[520] = "Unknown Error";
	codes_responses[521] = "Web Server Is Down";
	codes_responses[522] = "Connection Timed Out";
	codes_responses[523] = "Origin Is Unreachable";
	codes_responses[524] = "A Timeout Occurred";
	codes_responses[525] = "SSL Handshake Failed";
	codes_responses[526] = "Invalid SSL Certificate";
	codes_responses[527] = "Railgun Error";
	return codes_responses[code];
}

std::string ContentType(const std::string& extension) {
    std::map<std::string, std::string> contentTypes;
	contentTypes[".txt"] = "text/plain";
	contentTypes[".html"] = "text/html";
	contentTypes[".htm"] = "text/html";
	contentTypes[".css"] = "text/css";
	contentTypes[".js"] = "application/javascript";
	contentTypes[".json"] = "application/json";
	contentTypes[".xml"] = "application/xml";
	contentTypes[".pdf"] = "application/pdf";
	contentTypes[".zip"] = "application/zip";
	contentTypes[".gz"] = "application/gzip";
	contentTypes[".doc"] = "application/msword";
	contentTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	contentTypes[".xls"] = "application/vnd.ms-excel";
	contentTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	contentTypes[".ppt"] = "application/vnd.ms-powerpoint";
	contentTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	contentTypes[".jpg"] = "image/jpeg";
	contentTypes[".jpeg"] = "image/jpeg";
	contentTypes[".png"] = "image/png";
	contentTypes[".gif"] = "image/gif";
	contentTypes[".webp"] = "image/webp";
	contentTypes[".svg"] = "image/svg+xml";
	contentTypes[".mp3"] = "audio/mpeg";
	contentTypes[".ogg"] = "audio/ogg";
	contentTypes[".wav"] = "audio/wav";
	contentTypes[".mp4"] = "video/mp4";
	contentTypes[".webm"] = "video/webm";
	contentTypes[".ogv"] = "video/ogg";
	contentTypes[".tar"] = "application/x-tar";
	contentTypes[".7z"] = "application/x-7z-compressed";
	contentTypes[".rar"] = "application/x-rar-compressed";
	contentTypes[".md"] = "text/markdown";
	contentTypes[".rtf"] = "application/rtf";
	contentTypes[".sh"] = "application/x-sh";
	contentTypes[".py"] = "application/x-python";
	contentTypes[".jar"] = "application/x-java-archive";

    std::map<std::string, std::string>::const_iterator it = contentTypes.find(extension);
    if (it != contentTypes.end()) {
		return it->second;
	} else {
		return "application/octet-stream";  // Retourne ce type par défaut si l'extension est inconnue
	}
}

std::string	Server::getContentType(const std::string& path) {
	if (path.find('.') == std::string::npos) {
		return ("text/plain");
	}

	std::string extension = path.substr(path.find_last_of('.'));
	return ContentType(extension);
}

std::string	Server::getResponseHeader(Request *request, const std::string& path) {
	int code = atoi(request->getResponsCode().c_str());

	std::cerr << "debug code : " << code << std::endl;
	std::ostringstream oss;
	oss << request->getResponseFileSize();
    std::string header = "HTTP/1.1 " + request->getResponsCode() + " " + getMessageCode(code)+ "\r\n";
    header += "Content-Type: " + getContentType(path) + "\r\n";
	std::cerr << "Content type: " << getContentType(path) << std::endl;
    header += "Content-Length: " + oss.str() + "\r\n";
    header += "\r\n";
	return header;
}

std::string Server::_openResponseFile(Request *clientRequest)
{
	std::string	finalPath;

	if (clientRequest->getResponsCode() == "200") {
		if (clientRequest->path() == "/" || clientRequest->method() == DELETE_) {
			finalPath = _data->_root + _data->_index;
			if (access((finalPath).c_str(), F_OK | R_OK) == -1)
				clientRequest->setResponsCode("404");
			else
				clientRequest->openResponseFile(finalPath.c_str());
			std::cerr << "root : " + _data->_root << std::endl;
			std::cerr << "if root : " << finalPath << std::endl;
		}
		else {
			//TODO: check if is dir 
			// if yes path + index
			finalPath = _data->_root + clientRequest->path();
			std::cerr << "if no root : " << finalPath << std::endl;
			struct stat buf;
			if (access((finalPath).c_str(), F_OK | R_OK) == -1)
				clientRequest->setResponsCode("404");
			else if (stat((finalPath).c_str(), &buf) == -1)
				clientRequest->setResponsCode("500");
			else if (S_ISDIR(buf.st_mode))
			{
				finalPath += "/" + _data->_index;
				clientRequest->openResponseFile((finalPath + "/" + _data->_index).c_str());
			}
			else
				clientRequest->openResponseFile((finalPath).c_str());
		}
		if (clientRequest->getResponsCode() == "200" && clientRequest->responseFileOpen() == false)
			clientRequest->setResponsCode("404");
	}
	if (clientRequest->getResponsCode() != "200")
	{
		finalPath = "URIs/errors/" + clientRequest->getResponsCode() + ".html";
		clientRequest->openResponseFile(finalPath.c_str());
	}
	return finalPath;
}

void Server::sendResponse(int fd, Client *client) {
	Request *clientRequest = client->getRequest();

	std::cerr << "SEND RESPONSE" << std::endl;
	std::cerr << "RESPONSE CODE : " << clientRequest->getResponsCode() << std::endl;
	std::string	finalPath;
	
	if (clientRequest->responseFileOpen() == false)
	{
		finalPath = _openResponseFile(clientRequest);
		std::string	header = getResponseHeader(clientRequest, finalPath);
		if (send(fd, header.c_str(), header.size(), MSG_EOR) == -1) {
			client->setResponse("500");
			throw std::runtime_error("Can't send the message !");
		}
	}


	char	buffer[BUFFER_SIZE];
	std::size_t	n = 0;
	n += clientRequest->readResponseFile(buffer, BUFFER_SIZE);
	if (send(fd, buffer, n, MSG_NOSIGNAL) == -1)
	{
		clientRequest->closeResponseFile();
		client->setResponse("500");
		throw std::runtime_error("Can't send the message !");
	}
	clientRequest->addResponseFileTotalSend(n);
	if (clientRequest->getResponseFileTotalSend() == clientRequest->getResponseFileSize())
	{
		clientRequest->closeResponseFile();
		client->afterResponse();
	}
}

void Server::sendRedirect(std::string redirect, int fd, Client *client) {
	Request *clientRequest = client->getRequest();
	int code = atoi(clientRequest->getResponsCode().c_str());

	std::ostringstream oss;
	oss << "HTTP/1.1 " << code << " " << getMessageCode(code) << "\r\n"; 
	oss << "Location: " << redirect << "\r\n";
	oss << "Content-Length: 0\r\n";
	oss << "\r\n";

	std::string response = oss.str();
  
  	// std::cerr << "RESPONSE : " << std::endl;
	// std::cerr << response << std::endl;

	if (send(fd, response.c_str(), response.size(), MSG_NOSIGNAL) == -1)
	{
		client->setResponse("500");
		std::cerr << "test yesy yes" << std::endl;
		throw std::runtime_error("Can't send the message !");
	}
}

void Server::giveClientResponseByLocation(int fd) {
	Client* client = getClient(fd);
	Location location = _data->_locations[client->getRequest()->path()];
  
	if (client->isReadyToResponse() == false)
		return;

	if (!location.redirect().empty()) {
		client->getRequest()->setResponsCode("302");
		std::cerr << "redirected on : " + location.redirect() << std::endl;
		sendRedirect(location.redirect(), fd, client);
	}
	else {
		std::cerr << "no redirect field" << _data->_root + client->getRequest()->path() << std::endl;
	}
}

void setCookie(std::stringstream &response, std::string key, std::string value) {
	response
	<< "Set-Cookie: " << key << "=" << value << ";"
	<< "Max-Age=300"
	<< "\r\n";
}

void Server::handleAuth(Client* client) {
	Request* request = client->getRequest();
	std::stringstream response;

	client->getRequest()->setResponsCode("302");
	std::cerr << "je paasee == " << request->path() << std::endl;
	if (request->path() == "/auth/login"){
		response << "HTTP/1.1 " << request->getResponsCode() << " " << getMessageCode(std::atoi(request->getResponsCode().c_str())) << "\r\n"; 
		response << "Content-Length: 0\r\n";
		if (request->isKeyfindInHeader("Cookie") == false || request->find("Cookie").find("auth=true") == std::string::npos) {
			setCookie(response, "auth", "true");
		}
		std::cerr << "redirected on : " << MYCEOO << std::endl;
		response << "Location: " << MYCEOO << "\r\n"; 
		response << "\r\n";
		if (send(client->getClientFd(), response.str().c_str(), response.str().size(), MSG_EOR) == -1)
		{
			client->setResponse("500");
			throw std::runtime_error("Can't send the message !");
		}
	} else if (request->path() == "/auth/secret") {
		std::string header = request->getHeader();
		if (request->isKeyfindInHeader("Cookie") == true) {
			if (request->find("Cookie").find("auth=true") != std::string::npos) {
				std::cerr << "redirected on : " << SECRET << std::endl;
				sendRedirect(SECRET, client->getClientFd(), client);
			} else {
				std::cerr << "redirected on : " << SECRET << std::endl;
				sendRedirect(MYCEOC, client->getClientFd(), client);
			}
		}
		else
		{
			std::cerr << "redirected on : " << SECRET << std::endl;
			sendRedirect(MYCEOC, client->getClientFd(), client);
		}
	}

}

void Server::giveClientResponse(int fd) {
	Client *client;
	std::ifstream file;

	client = getClient(fd);
	if (client->isReadyToResponse() == false)
		return;
	if (client->getRequest()->getRedirect()) { 
		std::cerr << "location find ! with path == " + client->getRequest()->path() << std::endl;
		giveClientResponseByLocation(fd);
		client->afterResponse();
	}
	else if (std::strncmp(client->getRequest()->path().c_str(), "/auth/", 6) == 0 && client->getRequest()->getResponsCode() == "200")
	{
		handleAuth(client);
		client->afterResponse();
	}
	else if (client->getRequest()->isACgi() == true)
	{
		responseCGI(client);
		client->afterResponse();
	}
	else 
		sendResponse(fd, client); //this methode send response with appropriate code
}
