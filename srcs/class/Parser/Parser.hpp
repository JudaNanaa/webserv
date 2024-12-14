/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:32:51 by itahri            #+#    #+#             */
/*   Updated: 2024/12/14 17:22:55 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "../../../includes/includes.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct Location;
class Server;


struct Data {
  public:
    Data();
    ~Data();
    bool								checkLocation(const std::string &path) const;
	Location 							*getLocation(const std::string &path);
	const std::string					&getErrorPage(const std::string	&code) const;
	bool								errorPageIsSet(const std::string &code) const;

    int									_port;

    std::string							_serverName;
    std::string							_root;
    std::string							_uploadFolder;
    int									_allowedMethods;
    std::map<std::string, std::string>	_errorPages;
    bool								_autoIndex;
    std::map<std::string, std::string> 	_cgi;
    long								_clientMaxBodySize;
    std::map<std::string, Location>		_locations;
    std::vector<std::string>			_index;
};

class Pars {
  private:
    static void							parseConfigPath(std::string path);
    static void							handleLine(std::string &line, std::ifstream& configFile, Data *data, int &lineNumber);
    static void							parseServer(Server &serv, std::ifstream& configFile, int &lineNumber);
    static void							checkNecessary(Server& serv);
    static std::vector<Server>			parseConfigFile(std::ifstream &configFile, char **env);

  public:
    static std::vector<Server>			parse(std::string path, char **env);

    void								addPort(Data* data, const std::string &port);
    void								addServName(Data* data, const std::string &name);
    void								addRootDir(Data* data, const std::string &path);
    void								addUpFoldDir(Data* data, const std::string &path);
    void								addAllowedMethodes(Data* data, const std::string &w);
    void								addErrPage(Data* data, const std::string &path);
    void								addAutoIndex(Data* data, const std::string &w);
    void								addCgi(Data* data, const std::string &cgi);
    void								addClientMBodyS(Data* data, const std::string &size);
    void								addLocationDir(Data* data, const std::string &loc);
    void								addIndex(Data* data, const std::string &index);
};



#endif
