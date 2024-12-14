/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:50:51 by itahri            #+#    #+#             */
/*   Updated: 2024/12/14 17:23:08 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "../Server/Server.hpp"
#include "Location.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <sstream>
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

	while ((pos = line.find('\t', pos)) is_found) {
		line.replace(pos, 1, tabReplacement);
		pos += tabReplacement.length();
	}
	if (line.find(";") is_found)
		line.erase(line.find(';'), 1);
	trimn(line);
}

//check the path
void	Pars::parseConfigPath(std::string path) {
	if (path == ".conf") {
		throw std::invalid_argument("not a '.conf' file");
	} if (path.find('.') not_found) {	// no '.' found
		throw std::invalid_argument("not a '.conf' file");
	}

	if (path.substr(path.find_last_of('.')) != ".conf") {
		throw std::invalid_argument("not a '.conf' file");
	}
}

void Location::assignKeyValue(std::string &key, std::string &value) {
	static std::map<std::string, void (Location::*)(std::string&)> actions;
	if (actions.empty())
	{
		actions["index"] = &Location::index ;
		actions["root"] = &Location::root ;
		actions["cgi"] = &Location::addCgi ;
		actions["redirect"] = &Location::redirect;
		actions["uploads_folder"] = &Location::uploadFolder;
		actions["client_max_body_size"] = &Location::redirect;
		actions["auto_index"] = &Location::handleAutoIndex;
		actions["allowed_methods"] = &Location::handleAllowedMethods;
		actions["error_pages"] = &Location::setErrorPage;
	};

	std::map<std::string, void (Location::*)(std::string&)>::iterator it = actions.find(key);
	if (it != actions.end()) {
		void (Location::*func)(std::string&) = it->second;
		(this->*func)(value);
	} else
		throw std::invalid_argument("unknown assignment `" + key + "'");
}

void	Location::addLocationLine(std::string &line) {

	if (trim(line).empty())
		return ;
	else if (line.find(' ') not_found)
		throw std::invalid_argument("invalid line 2: " + line);
	else if (line.find(";") not_found)
		throw std::invalid_argument("`;' expected");

	std::string value = trim(line.substr(line.find(' ')));
	value.erase(value.find(";"));
	
	std::string key = trim(line.substr(0, line.find(' ')));

	if (key.empty())
		throw std::invalid_argument("missing key");
	else if (value.empty())
		throw std::invalid_argument("missing value");
	assignKeyValue(key, value);
}

void	_handleLocation(std::string &line, std::ifstream &configFile, Data& data, int &lineNumber) {
	Location	location;

	if (line.find(" ") not_found)
		throw std::invalid_argument("format: location <path>");

	std::string	location_path = trim(line.substr(line.find_first_of(" ")));
	location.location(location_path);
		
	std::getline(configFile, line);
	trimn(line);
	if (line.find("{") not_found)
		throw std::invalid_argument("`{' expected");
	++lineNumber;

	for (;std::getline(configFile, line); lineNumber++) {
		if (line.find("}") is_found)
			break;
		data._locations[location_path].addLocationLine(line);
	}
	data._locations[location_path].location(location_path);
}

// for each lines apply the associated function
void Pars::handleLine(std::string &line, std::ifstream& configFile, Data* data, int &lineNumber) {
	static std::map<std::string, void (Pars::*)(Data*, const std::string&)> functionMap;
	std::string	type;
	std::ostringstream oss;

	oss << lineNumber;
	if (functionMap.empty()) {
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
	}

	normalizeLine(line);
	printnl("line before == [" << line << "]");
	type = line.substr(0, line.find(' '));

	if (type == "location") {
		_handleLocation(line, configFile, *data, lineNumber);
		return ;
	}

	if (functionMap.find(trim(type)) == functionMap.end())	// type not in the map
		throw std::invalid_argument("unknow keyword: " + trim(type) + " at line " + oss.str());
	if (trim(&line[type.length()]).empty())
		throw std::invalid_argument("empty value for the key: " + type + " at line " + oss.str());
	Pars parsInstance;
	(parsInstance.*functionMap[trim(type)])(data, trim(&line[type.length()]));
	std::cout << trim(type) << ": " << &line[type.length()] << std::endl;
}

//for each server configuration check synthax and give each line to handleLine()
void	Pars::parseServer(Server &serv, std::ifstream& configFile, int &lineNumber) {
	std::string	line;
	Data *data = new Data();

	std::getline(configFile, line), trimn(line);
	if (line != "{")
		throw std::invalid_argument("invalid line 1");
	++lineNumber;
	for (;std::getline(configFile, line); lineNumber++) {
		if (line.find('}') is_found)
			break;
		if (!trim(line).empty())
			handleLine(line, configFile, data, lineNumber);
	}
	std::cout << "---------------------[NEW SERVER ADDED]---------------------" << std::endl;
	serv.addData(data);
}

void	Pars::checkNecessary(Server& serv) {
	if (serv._data->_root.empty())
		throw std::invalid_argument("no root");
	else if (serv._data->_port <= 1024 || serv._data->_port > 65535)
		throw std::invalid_argument("invalid port");
}

std::vector<Server> Pars::parseConfigFile(std::ifstream &configFile, char **env) {
	std::vector<Server> servVec;
	std::string	line;

	for (int ln = 0; std::getline(configFile, line); ln++) {
		if (line.empty() || trim(line).empty())
			continue;
		if (trim(line) == "server") {
			Server	newServ;
			parseServer(newServ, configFile, ln);
			newServ.setEnv(env), checkNecessary(newServ);
			servVec.push_back(newServ);
		}
	}
	if (servVec.empty())
		throw std::runtime_error("no server found");
  printnl("PARSING OK!");
  return servVec;
}

std::vector<Server> Pars::parse(std::string path, char **env) {
  parseConfigPath(path);
	std::ifstream configFile(path.c_str());

	if (configFile.fail())
	  throw std::invalid_argument("Can't open the config file");

	return parseConfigFile(configFile, env);
}
