/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confParse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:16:27 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/06 17:14:48 by itahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/Parser.hpp"
#include <exception>
#include <fstream>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>

void	parseConfigPath(std::string path) {
	if (path == ".conf") {
		throw std::invalid_argument("not a '.conf' file");
	} if (path.find('.') == std::string::npos) {	// no '.' found
		throw std::invalid_argument("not a '.conf' file");
	}

	if (path.substr(path.find('.')) != ".conf") {
		throw std::invalid_argument("not a '.conf' file");
	}

}

typedef  void (*function)(std::string&, Server&, int&);

void test(std::string &line, Server &serv, int &lineNumber);

void handleLine(std::string &line, Server &serv, int &lineNumber) {
	std::map<std::string, function> functionMap = {
		{"listen", test},
		{"server_names", test},
		{"root", test},
		{"uploads_folder", test},
		{"allowed_methods", test},
		{"auto_index", test},
		{"error_pages", test},
		{"cgi", test},
		{"client_max_body_size", test},
		{"location", test},
		{"index", test}
	};

	std::string	type = line.substr(0, line.find(" "));

	if (functionMap.find(type) == functionMap.end()) {	// type not in the map
		throw std::invalid_argument("unknow keyword: " + type);
	}

	functionMap[type](line, serv, lineNumber);
}

void	parseServer(Server &serv, std::ifstream& configFile, int &lineNumber) {
	std::string	line;

	std::getline(configFile, line);
	if (line != "{" || ) {
		throw std::invalid_argument("invalid line");
	}
	++lineNumber;

	for (std::getline(configFile, line);; lineNumber++) {
		if (line[0] != '\n' && line[0] != '\t') {	// line not empty and not indented
			throw std::invalid_argument("ambigious indentation");
		} else {
			line.erase(0, 1);
		}
		// if (line.) {
		// 	throw std::invalid_argument("ambigious indentation");
		// }
		handleLine(line, serv, lineNumber);

	}
	
}

std::string trim(const std::string& str) {
    // Supprimer les espaces en début
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return ""; // Chaîne vide si uniquement des espaces

    // Supprimer les espaces en fin
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    
    // Retourner la sous-chaîne sans espaces en début et en fin
    return str.substr(start, end - start + 1);
}

void	parseConfigFile(std::vector<Server>& serv, std::string configFilePath) {
	parseConfigPath(configFilePath);

	std::ifstream configFile(configFilePath);

	if (configFile.fail()) {
		throw std::invalid_argument("Can't open the config file");
	}

	std::string	line;

	for (int lineNumber = 0;;(std::getline(configFile, line), lineNumber++)) {
		// if (line == "server")
    Server	newServ;
    if (trim(line) == "server") {
      try {
        parseServer(newServ, configFile, lineNumber);
      } catch (std::exception& e) {
        throw std::invalid_argument("line " + std::to_string(lineNumber) + ": " + e.what());
      }
      serv.push_back(newServ);
    }
	}

}
