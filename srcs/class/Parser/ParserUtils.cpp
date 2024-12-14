/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:07:33 by itahri            #+#    #+#             */
/*   Updated: 2024/12/14 23:06:08 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "../Server/Server.hpp"
#include "../../../includes/includes.hpp"
#include <exception>
#include <fstream>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <cstddef>


void Pars::addPort(Data* data, const std::string &port) {
  if (!port.empty())
    data->_port = std::atoi(port.c_str());
}

void Pars::addServName(Data* data, const std::string &name) {
  if (!name.empty())
    data->_serverName = name;
}


void Pars::addRootDir(Data* data, const std::string &path) {
	data->_root = path;
	if (path.find("/") != path.length() - 1)
		data->_root.push_back('/');
}

void Pars::addUpFoldDir(Data* data, const std::string &path) {
	if (!path.empty())
		data->_uploadFolder = path;
}

void Pars::addAllowedMethodes(Data* data, const std::string &str) {
	int	methods = 0;
	std::string tmp;

	tmp = str;
	if (tmp.find(";") is_found)
		tmp.erase(tmp.find(';'), 1);
	if (tmp.find("GET") is_found) {
		methods = methods | GET_;
		tmp.erase(tmp.find("GET"), 3);
	}
	if (tmp.find("POST") is_found) {
		methods = methods | POST_;
		tmp.erase(tmp.find("POST"), 4);
	}
	if (tmp.find("DELETE") is_found) {
		methods = methods | DELETE_;
		tmp.erase(tmp.find("DELETE"), 6);
	}

	if (trim(tmp).empty() == false) // still words after deleting valids methods 
		throw std::invalid_argument("invalid methods");
	data->_allowedMethods = methods;
}

void Pars::addErrPage(Data* data, const std::string &path) {
	std::vector<std::string> vec;

	vec = split(path, " ");
	if (vec.size() != 2)
		throw std::invalid_argument("Invalid argument error_pages");
	if (allDigit(vec[0].data()) == false)
		throw std::invalid_argument("Not a number at error_pages");
    data->_errorPages[vec[0]] = vec[1];
}


void Pars::addAutoIndex(Data* data, const std::string &w) {
  if (w == "on")
    data->_autoIndex = true;
}


void Pars::addCgi(Data* data, const std::string &cgi) {
  //TODO: add cgi logic
  if (cgi.find(" ") is_found) {
    data->_cgi[cgi.substr(0, cgi.find(' '))] = trim(cgi.substr(cgi.find(' ')));
  }
}


void Pars::addClientMBodyS(Data* data, const std::string &size) {
  if (!size.empty())
    data->_clientMaxBodySize = std::atoi(size.c_str());
}

void Pars::addIndex(Data* data, const std::string &index) {
    data->_index = split(index, " ");
}
