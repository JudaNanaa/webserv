/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:15:03 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/09 20:07:241 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp" 
#include "../../../includes/utils.hpp" 
#include "../Server/Server.hpp"
#include <map>
#include <fstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

Request::Request(Client *client, Server *server)
    : RawBits(),
		_responseCgi(false),
		_state(ON_HEADER),
		_method(0),
		_ResponsCode("200"),
		_contentLenght(-1),
		_server(server),
		_client(client),
		_isRedirect(false)
{

}

Request::~Request()
{

}

std::ostream& operator<<(std::ostream& os, const Request& request ) {
	os
	<< "Request {\n"
	<< "\t" << "method: ";
	if (request.method() & GET_) {
		os << "GET";
	} else if (request.method() & POST_) {
		os << "POST";
	} else if (request.method() & DELETE_) {
		os << "DELETE";
	} else if (request.method() & OPTIONS_) {
		os << "OPTIONS";
	}  else {
		os << "none";
	} os << std::endl;

	os
	<< "\t" << "path: " << request.path() << std::endl
	<< std::endl;

	std::map<std::string, std::string>::const_iterator ite = request._others.end();
	for (std::map<std::string, std::string>::const_iterator it = request._others.begin(); it != ite; it++) {
		os << "\t" << it->first << ": " << it->second << std::endl;
	}

	os << "}" << std::endl;

	return os;
}
