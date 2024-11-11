/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:50:51 by itahri            #+#    #+#             */
/*   Updated: 2024/11/10 19:50:06 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Parser.hpp"
#include "../../../includes/Server.hpp"
#include "../../../includes/Location.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>

void debugPrint(std::string str) {
	for (int i = 0; str[i]; i++) {
	  if (str[i] == ' ')
	    std::cout << "~";
	  else if (str[i] == '\t')
	    std::cout << "\\t";
	  else
	    std::cout << str[i];
	}
}

void normalizeLine(std::string &line) {
	const std::string tabReplacement = "    ";
	size_t pos = 0;

	while ((pos = line.find('\t', pos)) != std::string::npos) {
	  line.replace(pos, 1, tabReplacement);
	  pos += tabReplacement.length();
	}
  if (line.find(";") != std::string::npos)
    line.erase(line.find(';'), 1);
	trimn(line);
}

//check the path
void	Pars::parseConfigPath(std::string path) {
	if (path == ".conf") {
		throw std::invalid_argument("not a '.conf' file");
	} if (path.find('.') == std::string::npos) {	// no '.' found
		throw std::invalid_argument("not a '.conf' file");
	}

	if (path.substr(path.find('.')) != ".conf") {
		throw std::invalid_argument("not a '.conf' file");
	}
}

void	addLocationLine(std::string &line, Location& location) {

	if (line.empty() || trim(line).empty())
		return ;
	else if (line.find(' ') == std::string::npos)
		throw std::invalid_argument("invalid line 2: " + line);
	else if (line.find(";") == std::string::npos)
		throw std::invalid_argument("`;' expected");

	std::string value = trim(line.substr(line.find(' ')));
	value.erase(value.find(";"));
	
	std::string key = trim(
		line.substr(0, line.find(' ')));

	if (key.empty())
		throw std::invalid_argument("missing key");
	else if (value.empty()) {
		throw std::invalid_argument("missing value");
	}

	// std::cerr << "KEY == '" << key<< "' | VALUE == '" << value << "'" << std::endl;

	if (key == "index") {	//			INDEX
		location.index(value);
	} else if (key == "root") {	//			ROOT
		location.root(value);
	} else if (key == "cgi") {	//			CGI
		location.cgi(value);
	} else if (key == "redirect") {	//			REDIRECT
		location.redirect(value);
	} else if (key == "allowed_methods") {	//			ALLOWED METHODS
		int methods = 0;
		if (value.find("GET") != std::string::npos) {
			methods = methods | GET_;
		} if (value.find("POST") != std::string::npos) {
			methods = methods | POST_;
		} if (value.find("DELETE") != std::string::npos) {
			methods = methods | DELETE_;
		} if (value.find("OPTIONS") != std::string::npos) {
			methods = methods | OPTIONS_;
		} 
		
		if (methods == 0) {	// no methods found
			throw std::invalid_argument("GET, DELETE or POST expected");
		} else {
			location.allowedMethods(methods);
		}
			
	} else if (key == "uploads_folder") {	//			UPLOADS_FOLDER
		location.uploadFolder(value);
	} else if (key == "client_max_body_size") {	//			CLIENT_MAX_BODY_SIZE
		int	int_value = std::atoi(value.c_str());
		if (int_value <= 0 || value.find_first_not_of("0123456789") != std::string::npos)
			throw std::invalid_argument("invalid value");
		location.maxBodySize(int_value);
	} else if (key == "auto_index") {	//			AUTO_INDEX
		if (value == "on") {
			location.autoIndex(true);
		} else if (value == "off") {
			location.autoIndex(false);
		} else
			throw std::invalid_argument("invalid value: expected 'on' or 'off'");
	} else if (key == "file_upload") {	//			FILE_UPLOAD
		if (value == "on") {
			location.fileUpload(true);
		} else if (value == "off") {
			location.fileUpload(false);
		} else
			throw std::invalid_argument("invalid value: expected 'on' or 'off'");
	} else {
		throw std::invalid_argument("unknow assignement `" + key + "'");
	}
}

void	handleLocation(std::string &line, std::ifstream &configFile, Data& data, int &lineNumber) {
	if (line.find(" ") == std::string::npos) {
		throw std::invalid_argument("format: location <path>");
	}

	Location	location;
	std::string	location_path = trim(line.substr(
		line.find_first_of(" ")
	));
	// location_path = location_path.erase(location_path.find(" "));
	location.location(location_path);
		
	std::getline(configFile, line);
	if (line.find("{") == std::string::npos) {
		throw std::invalid_argument("`{' expected");
	} ++lineNumber;

	for (;std::getline(configFile, line); lineNumber++) {
		// std::cerr << "{" << lineNumber << "}'" << trim(line) << "'" << std::endl;
		if (line.find("}") != std::string::npos) {
			break;
		}
		
		addLocationLine(line, location);
	}
	std::cout << location << std::endl;
	data._locations[location_path] = location;
}

// for each lines apply the associated function
void Pars::handleLine(std::string &line, std::ifstream& configFile, Data* data, int &lineNumber) {
	// (void)lineNumber;
	/*typedef void (Pars::*FunctionType)(Data*, std::string);*/

	if (trim(line).empty()) {
		return ;
	}

	std::map<std::string, void (Pars::*)(Data*, std::string)> functionMap;

  (void)lineNumber;

	functionMap["listen"] = &Pars::addPort;
	functionMap["server_names"] = &Pars::addServName;
	functionMap["root"] = &Pars::addRootDir;
	functionMap["uploads_folder"] = &Pars::addUpFoldDir;
	functionMap["allowed_methods"] = &Pars::addAllowedMethodes;
	functionMap["auto_index"] = &Pars::addAutoIndex;
	functionMap["error_pages"] = &Pars::addErrPage;
	functionMap["cgi"] = &Pars::addCgi;
	functionMap["client_max_body_size"] = &Pars::addClientMBodyS;
	functionMap["index"] = &Pars::addIndex;

	std::string	type;

	normalizeLine(line);
	type = line.substr(0, line.find(' '));
	// std::cout << "{" << lineNumber << ": " << type << "}" << "'->";
	// debugPrint(line);
	// std::cout << "<-'" << std::endl;

	if (type == "location") {
		handleLocation(line, configFile, *data, lineNumber);
		return ;
	}

	// std::cout << "{" << lineNumber << "}" << "[" << trim(type) << "]" << std::endl;
	if (functionMap.find(trim(type)) == functionMap.end()) {	// type not in the map
		throw std::invalid_argument("unknow keyword: " + trim(type));
	}
	Pars parsInstance; //for now is the only method to do what i want i will change this soon
	(parsInstance.*functionMap[trim(type)])(data, trim(line.substr(line.find(" "))));
	std::cout << trim(type) << ": " << line.substr(line.find(" ")) << std::endl;;
}

//for each server configuration check synthax and give each line to handleLine()
void	Pars::parseServer(Server &serv, std::ifstream& configFile, int &lineNumber) {
	std::string	line;
	Data *data = new Data();

	std::getline(configFile, line);
	if (line != "{") {
		throw std::invalid_argument("invalid line 1");
	}

	++lineNumber;
	// std::cout << lineNumber << std::endl;

	for (;std::getline(configFile, line); lineNumber++) {
	//   std::cout << "debug : " << line << std::endl;
	  if (line.find('}') != std::string::npos)
	    break;
	  if (!line.empty())
		  handleLine(line, configFile, data, lineNumber);
	}
	serv.addData(data);
}

std::vector<Server> Pars::parseConfigFile(std::string configFilePath) {
	std::vector<Server> servVec;
	std::string	line;

	parseConfigPath(configFilePath);
	std::ifstream configFile(configFilePath.c_str());

	if (configFile.fail()) {
	  throw std::invalid_argument("Can't open the config file");
	}
	for (int lineNumber = 0;std::getline(configFile, line); lineNumber++) {
	  if (line.empty() || trim(line).empty()) continue;
	  if (trim(line) == "server") {
	    std::cout << "---------------------[NEW SERVER ADDED]---------------------" << std::endl;
	    Server	newServ;
	    try {
	      parseServer(newServ, configFile, lineNumber);
	    } catch (std::exception& e) {
	      throw std::invalid_argument(e.what());
	    }
	    servVec.push_back(newServ);
	  }
	}
	return servVec;
}

std::vector<Server> Pars::parse(std::string path) {
	return parseConfigFile(path);
}





