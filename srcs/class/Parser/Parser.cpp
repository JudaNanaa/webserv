/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itahri <itahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:50:51 by itahri            #+#    #+#             */
/*   Updated: 2024/11/06 17:16:07 by itahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Parser.hpp"
#include "../../../includes/Server.hpp"
#include <exception>
#include <fstream>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>


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

//for each lines apply the associated function
void Pars::handleLine(std::string &line, Data* data, int &lineNumber) {
  (void)lineNumber;
	/*typedef void (Pars::*FunctionType)(Data*, std::string);*/
  std::map<std::string, void (Pars::*)(Data*, std::string)> functionMap;

  functionMap["listen"] = &Pars::addPort;
  functionMap["server_names"] = &Pars::addServName;
  functionMap["root"] = &Pars::addRootDir;
  functionMap["uploads_folder"] = &Pars::addUpFoldDir;
  functionMap["allowed_methods"] = &Pars::addAllowedMethodes;
  functionMap["auto_index"] = &Pars::addAutoIndex;
  functionMap["error_pages"] = &Pars::addErrPage;
  functionMap["cgi"] = &Pars::addCgi;
  functionMap["client_max_body_size"] = &Pars::addClientMBodyS;
  functionMap["location"] = &Pars::addLocationDir;
  functionMap["index"] = &Pars::addIndex;

	std::string	type = line.substr(0, line.find(" "));

	if (functionMap.find(trim(type)) == functionMap.end()) {	// type not in the map
		throw std::invalid_argument("unknow keyword: " + type);
	}
  Pars parsInstance; //for now is the only method to do what i want i will change this soon
  (parsInstance.*functionMap[type])(data, line.substr(line.find(" "), line.size()));
}

//for each server configuration check synthax and give each line to handleLine()
void	Pars::parseServer(Server &serv, std::ifstream& configFile, int &lineNumber) {
	std::string	line;
  Data *data = new Data();

	std::getline(configFile, line);
	if (line != "{") {
		throw std::invalid_argument("invalid line");
	}
	
	++lineNumber;

	for (std::getline(configFile, line);; lineNumber++) {
		/*if (line[0] != '\n' && line[0] != '\t' && line[0] != ' ') {	// line not empty and not indented*/
		/*	throw std::invalid_argument("ambigious indentation  : [" + line + "]");*/
		/*} else {*/
		/*	line.erase(0, 1);*/
		/*}*/
		// if (line.) {
		// 	throw std::invalid_argument("ambigious indentation");
		// }
    std::cout << "debug : \n"
      << "\t[line] " << line
      << std::endl;
    std::cout << "\t [data] " << data << std::endl;
		handleLine(line, data, lineNumber);
	}
  serv.addData(data);
}



std::vector<Server>*	Pars::parseConfigFile(std::string configFilePath) {
  std::vector<Server>* servVec = new std::vector<Server>;
  std::string	line;

  parseConfigPath(configFilePath);
  std::ifstream configFile(configFilePath.c_str());

  if (configFile.fail()) {
    throw std::invalid_argument("Can't open the config file");
  }
  for (int lineNumber = 0;;(std::getline(configFile, line), lineNumber++)) {
    // if (line == "server")
    if (trim(line) == "server") {
      Server	newServ;
      try {
        parseServer(newServ, configFile, lineNumber);
      } catch (std::exception& e) {
        throw std::invalid_argument(e.what());
      }
      servVec->push_back(newServ);
    }
    // else
    //add a server in vector for each server config in file
    // error
  }
}


std::vector<Server>* Pars::parse(std::string path) {
  return parseConfigFile(path);
}

