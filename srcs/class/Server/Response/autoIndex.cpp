/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:20:23 by madamou           #+#    #+#             */
/*   Updated: 2024/12/14 16:24:28 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include <dirent.h>

void Server::_generateAutoIndex(Client *client, const std::string &directoryPath) {
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
		return;
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
		std::string path = client->getRequest()->path() + "/" + name;
		formatSlash(path);
		html += "        <button class=\"" + colorClass + "\" onclick=\"window.location.href='" + path + "'\">" + name + "</button>\r\n";
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
}

std::vector<std::string> Server::_getIndex(Request *clientRequest)
{
	std::vector<std::string> index;

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

std::string Server::_checkIndex(const std::vector<std::string> &indexVec, const std::string &directoryPath)
{
	for (int i = 0, len = indexVec.size(); i < len; i++)
	{
		if (access((directoryPath + '/' + indexVec[i]).data(), F_OK | R_OK) == 0)
			return directoryPath + '/' + indexVec[i];
	}
	return "";
}

std::string Server::_manageIfDirectory(Client *client, Request *clientRequest, std::string &directoryPath)
{
	std::vector<std::string> indexVec;
	std::string finalPath;

	indexVec = _getIndex(clientRequest);
	if (indexVec.empty() is true)
	{
		if (_ifAutoIndex(clientRequest))
			_generateAutoIndex(client, directoryPath);
		else 
			clientRequest->setResponsCode("403");
	}
	else 
	{
		finalPath = _checkIndex(indexVec, directoryPath);
		if (finalPath.empty())
		{
			if (_ifAutoIndex(clientRequest))
				_generateAutoIndex(client, directoryPath);
			else 
				clientRequest->setResponsCode("404");
		}
		else
			clientRequest->openResponseFile(finalPath.c_str());
	}
	return finalPath;
}
