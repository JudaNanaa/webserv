/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:07:33 by itahri            #+#    #+#             */
/*   Updated: 2024/11/08 21:00:36 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Parser.hpp"
#include "../../../includes/Server.hpp"
#include "../../../includes/includes.hpp"
#include <exception>
#include <fstream>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <cstddef>


void Pars::addPort(Data* data, std::string port) {
  if (!port.empty())
    data->_port = std::atoi(port.c_str());
}

void Pars::addServName(Data* data, std::string name) {
  if (!name.empty())
    data->_serverName = name;
}


void Pars::addRootDir(Data* data, std::string path) {
  if (!path.empty())
    data->_root = path;
}


void Pars::addUpFoldDir(Data* data, std::string path) {
  if (!path.empty())
    data->_index = path;
}


void Pars::addAllowedMethodes(Data* data, std::string w) {
  if (w.find("GET") != std::string::npos) 
    data->_allowedMethods += GET_;
  if (w.find("POST") != std::string::npos) 
    data->_allowedMethods += POST_;
  if (w.find("DELETE") != std::string::npos) 
    data->_allowedMethods += DELETE_;
}


void Pars::addErrPage(Data* data, std::string path) {
  if (!path.empty())
    data->_errorPage = path;
}


void Pars::addAutoIndex(Data* data, std::string w) {
  if (w == "on")
    data->_autoIndex = true;
}


void Pars::addCgi(Data* data, std::string cgi) {
  //TODO: add cgi logic
  data->_cgi = cgi;
}


void Pars::addClientMBodyS(Data* data, std::string size) {
  if (!size.empty())
    data->_clientMaxBodySize = std::atoi(size.c_str());
}

void Pars::addIndex(Data* data, std::string index) {
  if (!index.empty())
    data->_index = index;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return ""; // Chaîne vide si uniquement des espaces
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

void trimn(std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return ; // Chaîne vide si uniquement des espaces
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    str.erase(end + 1);
    str.erase(0, start);
}
