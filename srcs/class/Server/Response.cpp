/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 01:01:30 by madamou           #+#    #+#             */
/*   Updated: 2024/12/13 19:09:35 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
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
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

#define SECRET "https://www.cat29.fr/riche-et-independant/"
#define SINGE "https://media.tenor.com/_uIJwdpxI8UAAAAM/mono-serio.gif"
#define MYCEOO "https://pbs.twimg.com/media/EaoNasxXYAUE8pA.jpg"
#define MYCEOC "https://pbs.twimg.com/media/EaoNYXkXkAMHJJK.jpg"


const std::string &getMessageCode(const int &code) {
	static std::map<int, std::string> codes_responses;

	if (codes_responses.empty())
	{
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
	}
	return codes_responses[code];
}

const std::string ContentType(const std::string& extension) {
    static std::map<std::string, std::string> contentTypes;

	if (contentTypes.empty())
	{
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
	}

    std::map<std::string, std::string>::const_iterator it = contentTypes.find(extension);
    if (it != contentTypes.end())
		return it->second;
	return "application/octet-stream";  // Retourne ce type par défaut si l'extension est inconnue
}

std::string Server::_generateAutoIndex(Client *client, const std::string &directoryPath) {
	std::string html;

	html += "<!DOCTYPE html>\r\n";
	html += "<html lang=\"en\">\r\n";
	html += "<head>\r\n";
	html += "    <meta charset=\"UTF-8\">\r\n";
	html += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n";
	html += "    <title>Index of " + directoryPath + "</title>\r\n";
	html += "    <style>\r\n";
	html += "        body {\r\n";
	html += "            font-family: Arial, sans-serif;\r\n";
	html += "            margin: 0;\r\n";
	html += "            padding: 0;\r\n";
	html += "            display: flex;\r\n";
	html += "            flex-direction: column;\r\n";
	html += "            align-items: center;\r\n";
	html += "            justify-content: center;\r\n";
	html += "            height: 100vh;\r\n";
	html += "            background: linear-gradient(135deg, #1e3c72, #2a5298, #f6d365, #fda085);\r\n";
	html += "            background-size: 300% 300%;\r\n";
	html += "            animation: gradientAnimation 15s ease infinite;\r\n";
	html += "            color: #fff;\r\n";
	html += "        }\r\n";
	html += "        h1 {\r\n";
	html += "            color: #fff;\r\n";
	html += "            font-size: 24px;\r\n";
	html += "            margin-top: 20px;\r\n";
	html += "            margin-bottom: 20px;\r\n";
	html += "            border-bottom: 3px solid #fff;\r\n";
	html += "            padding-bottom: 10px;\r\n";
	html += "            text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3);\r\n";
	html += "        }\r\n";
	html += "        .button-container {\r\n";
	html += "            display: flex;\r\n";
	html += "            flex-direction: column;\r\n";
	html += "            align-items: center;\r\n";
	html += "            gap: 10px;\r\n";
	html += "        }\r\n";
	html += "        .button-container button {\r\n";
	html += "            font-size: 16px;\r\n";
	html += "            padding: 10px 20px;\r\n";
	html += "            border: none;\r\n";
	html += "            border-radius: 8px;\r\n";
	html += "            cursor: pointer;\r\n";
	html += "            color: #fff;\r\n";
	html += "            font-weight: bold;\r\n";
	html += "            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);\r\n";
	html += "            transition: transform 0.2s ease, box-shadow 0.2s ease;\r\n";
	html += "        }\r\n";
	html += "        .button-container button:hover {\r\n";
	html += "            transform: scale(1.1);\r\n";
	html += "            box-shadow: 0 6px 10px rgba(0, 0, 0, 0.15);\r\n";
	html += "        }\r\n";
	html += "        .color-1 { background-color: #ff6b6b; }\r\n";
	html += "        .color-1:hover { background-color: #ff4b4b; }\r\n";
	html += "        .color-2 { background-color: #48dbfb; }\r\n";
	html += "        .color-2:hover { background-color: #33c7f2; }\r\n";
	html += "        .color-3 { background-color: #1dd1a1; }\r\n";
	html += "        .color-3:hover { background-color: #10b892; }\r\n";
	html += "        .color-4 { background-color: #feca57; }\r\n";
	html += "        .color-4:hover { background-color: #fdb844; }\r\n";
	html += "        .color-5 { background-color: #ff9ff3; }\r\n";
	html += "        .color-5:hover { background-color: #ff7eea; }\r\n";
	html += "        @keyframes gradientAnimation {\r\n";
	html += "            0% { background-position: 0% 50%; }\r\n";
	html += "            50% { background-position: 100% 50%; }\r\n";
	html += "            100% { background-position: 0% 50%; }\r\n";
	html += "        }\r\n";
	html += "    </style>\r\n";
	html += "</head>\r\n";
	html += "<body>\r\n";
	html += "    <h1>Index of " + directoryPath + "</h1>\r\n";
    html += "    <div class=\"button-container\">\r\n";

    DIR *currentDir = opendir(directoryPath.c_str());
    if (currentDir == NULL) {
        std::cerr << "Error: Could not open directory " << directoryPath << std::endl;
        client->setResponse("500");
        return "";
    }

    struct dirent *elem;
    int colorIndex = 2; // Start with the next color
    while ((elem = readdir(currentDir)) != NULL) {
        std::string name = elem->d_name;

        if (name == "." || name == "..")
            continue;
        std::ostringstream oss;

        oss << colorIndex;
        std::string colorClass = "color-" + oss.str();
        colorIndex = (colorIndex % 5) + 1;

        html += "        <button class=\"" + colorClass + "\" onclick=\"window.location.href='" + client->getRequest()->path() + "/" + name + "'\">" + name + "</button>\r\n";
    }
    closedir(currentDir);

    html += "    </div>\r\n";
    html += "    <div style=\"margin-top: 20px; text-align: center;\">\r\n";
    html += "        <button class=\"color-1\" onclick=\"window.history.back()\">Go Back</button>\r\n";
    html += "    </div>\r\n";

    html += "</body>\r\n";
    html += "</html>\r\n";


	Request *clientRequest = client->getRequest();
	int code = atoi(clientRequest->getResponsCode().c_str());

	std::cerr << "AUTO INDEX" << std::endl;
	std::ostringstream oss;
	oss << html.length();
    std::string header = "HTTP/1.1 " + clientRequest->getResponsCode() + " " + getMessageCode(code)+ "\r\n";
    header += "Content-Type: " + std::string("text/html") + "\r\n";
    header += "Content-Length: " + oss.str() + "\r\n";
    header += "\r\n";

	if (send(client->getClientFd(), (header + html).c_str(), header.size() + html.size(), MSG_NOSIGNAL) == -1)
		throw std::runtime_error("Can't send the message !");
	clientRequest->setState(SEND);
    return html;
}

std::string	Server::_getContentType(const std::string& path) {
	if (path.find('.') not_found) {
		return ("text/plain");
	}

	std::string extension = path.substr(path.find_last_of('.'));
	return ContentType(extension);
}

std::string	Server::_getResponseHeader(Request *request, const std::string& path) {
	int code = atoi(request->getResponsCode().c_str());

	std::cerr << "debug code : " << code << std::endl;
	std::ostringstream oss;
	oss << request->getResponseFileSize();
    std::string header = "HTTP/1.1 " + request->getResponsCode() + " " + getMessageCode(code)+ "\r\n";
    header += "Content-Type: " + _getContentType(path) + "\r\n";
	std::cerr << "Content type: " << _getContentType(path) << std::endl;
    header += "Content-Length: " + oss.str() + "\r\n";
    header += "\r\n";
	return header;
}

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
	return finalPath;
}

std::string Server::_getIndex(Request *clientRequest)
{
	std::string index;

	if (clientRequest->getRequestType() is LOCATION) {
		Location *location = clientRequest->getLocation();
		index = location->index().empty() ? _data->_index : location->index();
	}
	else if (clientRequest->getRequestType() is DEFAULT)
		index = _data->_index;
	return index;
}

bool Server::_ifAutoIndex(Request *clientRequest)
{
	bool autoIndex;
	
	if (clientRequest->getRequestType() is LOCATION)
	{
		Location *location = clientRequest->getLocation();
		autoIndex = location->autoIndex() < 0 ? _data->_autoIndex : location->autoIndex();
	}
	else
		autoIndex = _data->_autoIndex;
	return autoIndex;
}

void Server::_manageIfDirectory(Client *client, Request *clientRequest, std::string &finalPath)
{
	std::string index;

	index = _getIndex(clientRequest);
	if (index.empty() is true)
	{
		
		if (_ifAutoIndex(clientRequest))
			_generateAutoIndex(client, finalPath);
		else 
			clientRequest->setResponsCode("403");				
	}
	else 
	{
		finalPath += "/" + index;
		clientRequest->openResponseFile(finalPath.c_str());
		if (clientRequest->responseFileOpen() is false)
			_generateAutoIndex(client, finalPath.erase(finalPath.find_last_of('/')));
	}
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
		_manageIfDirectory(client, clientRequest, finalPath);
	else
		clientRequest->openResponseFile((finalPath).c_str());
	if (clientRequest->getResponsCode() is "200" && clientRequest->responseFileOpen() is false)
		clientRequest->setResponsCode("404");
	return finalPath;
}

std::string Server::_openResponseFile(Request *clientRequest, Client* client)
{
	std::string	finalPath;

	if (clientRequest->getResponsCode() is "200")
		finalPath = _normalOpenFile(clientRequest, client);
	if (clientRequest->getResponsCode() != "200")
	{
		finalPath = "URIs/errors/" + clientRequest->getResponsCode() + ".html";
		clientRequest->openResponseFile(finalPath.c_str());
	}
	return finalPath;
}

void Server::_sendResponse(const int &fd, Client *client) {
	Request *clientRequest = client->getRequest();
	char	buffer[BUFFER_SIZE];
	std::size_t	n = 0;

	std::cerr << "SEND RESPONSE" << std::endl;
	std::cerr << "RESPONSE CODE : " << clientRequest->getResponsCode() << std::endl;
	std::string	finalPath;
	
	if (clientRequest->responseFileOpen() is false)
	{
		finalPath = _openResponseFile(clientRequest, client);
		if (clientRequest->getState() is SEND)
			return;
		std::string	header = _getResponseHeader(clientRequest, finalPath);
		if (send(fd, header.c_str(), header.size(), MSG_NOSIGNAL) is -1) {
			client->setResponse("500");
			throw std::runtime_error("Can't send the message !");
		}
	}

	n += clientRequest->readResponseFile(buffer, BUFFER_SIZE);
	if (send(fd, buffer, n, MSG_NOSIGNAL) is -1)
	{
		clientRequest->closeResponseFile();
		client->setResponse("500");
		throw std::runtime_error("Can't send the message !");
	}
	clientRequest->addResponseFileTotalSend(n);
	if (clientRequest->getResponseFileTotalSend() is clientRequest->getResponseFileSize())
	{
		clientRequest->closeResponseFile();
		clientRequest->setState(SEND);
	}
}

void Server::_sendRedirect(const std::string &redirect, const int &fd, Client *client) {
	Request *clientRequest = client->getRequest();
	int code = atoi(clientRequest->getResponsCode().c_str());

	std::ostringstream oss;
	oss << "HTTP/1.1 " << code << " " << getMessageCode(code) << "\r\n"; 
	oss << "Location: " << redirect << "\r\n";
	oss << "Content-Length: 0\r\n";
	oss << "\r\n";

	std::string response = oss.str();

	if (send(fd, response.c_str(), response.size(), MSG_NOSIGNAL) is -1)
	{
		client->setResponse("500");
		throw std::runtime_error("Can't send the message !");
	}
	clientRequest->setState(SEND);
}

void setCookie(std::stringstream &response, std::string key, std::string value) {
	response
	<< "Set-Cookie: " << key << "=" << value << ";"
	<< "Max-Age=300"
	<< "\r\n";
}

void Server::_handleAuth(Client* client) {
	Request* request = client->getRequest();
	std::stringstream response;

	client->getRequest()->setResponsCode("302");
	if (request->path() is "/auth/login"){
		response << "HTTP/1.1 " << request->getResponsCode() << " " << getMessageCode(std::atoi(request->getResponsCode().c_str())) << "\r\n"; 
		response << "Content-Length: 0\r\n";
		if (request->isKeyfindInHeader("Cookie") is false || request->find("Cookie").find("auth=true") not_found) {
			setCookie(response, "auth", "true");
		}
		std::cerr << "redirected on : " << MYCEOO << std::endl;
		response << "Location: " << MYCEOO << "\r\n";
		response << "\r\n";
		if (send(client->getClientFd(), response.str().c_str(), response.str().size(), MSG_EOR) is -1) {
			client->setResponse("500");
			throw std::runtime_error("Can't send the message !");
		}
	} else if (request->path() is "/auth/secret") {
		std::string header = request->getHeader();
		if (request->isKeyfindInHeader("Cookie") is true) {
			if (request->find("Cookie").find("auth=true") is_found) 
				_sendRedirect(SECRET, client->getClientFd(), client);
			else 
				_sendRedirect(MYCEOC, client->getClientFd(), client);
		}
		else
			_sendRedirect(MYCEOC, client->getClientFd(), client); 
	}
	request->setState(SEND);
}

void Server::_sendResponseLocation(Client *client)
{
	Request *clientRequest;
	Location *location;

	clientRequest = client->getRequest();
	location = clientRequest->getLocation();
	if (clientRequest->getRedirect())
	{
		clientRequest->setResponsCode("302");
		std::cerr << "redirected on : " + location->redirect() << std::endl;
		_sendRedirect(location->redirect(), client->getClientFd(), client);
	}
	else if (clientRequest->getRequestType() is CGI)
	{
		if (_checkCgi(client) is FINISHED)
			_responseCGI(client);
	}
	else
	 	_sendResponse(client->getClientFd(), client);
}

void Server::_sendResponseDefault(Client *client)
{
	Request *clientRequest;

	clientRequest = client->getRequest();
	if (clientRequest->getRequestType() is CGI)
	{
		if (_checkCgi(client) is FINISHED)
			_responseCGI(client);
	}
	else
	 	_sendResponse(client->getClientFd(), client);
}

t_state Server::giveClientResponse(const int &fd) {
	Client *client;
	std::ifstream file;

	client = _getClient(fd);
	if (std::strncmp(client->getRequest()->path().c_str(), "/auth/", 6) is 0 && client->getRequest()->getResponsCode() is "200")
		_handleAuth(client);
	if (client->getRequest()->getRequestType() is LOCATION)
		_sendResponseLocation(client);
	else 
		_sendResponseDefault(client); //this methode send response with appropriate code
	if (client->getRequest()->getState() == SEND)
	{
		client->afterResponse();
		return SEND;
	}
	return RESPONSE;
}
