/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:25:20 by madamou           #+#    #+#             */
/*   Updated: 2024/12/15 21:23:50 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void setCookie(std::stringstream &response, std::string key, std::string value) {
	response
	<< "Set-Cookie: " << key << "=" << value << ";"
	<< "Max-Age=300"
	<< "\r\n";
}

void Server::_handleAuth(Client* client) {
	Request* request = client->getRequest();
	std::stringstream response;

	if (request->path() is "/auth/login") {
		printnl("click on login !!");
		request->setResponsCode("302");
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
		printnl("click on secret !!");
		std::string header = request->getHeader();
		if (request->isKeyfindInHeader("Cookie") is true) {
			if (request->find("Cookie").find("auth=true") is_found)
				_sendRedirect(client, SECRET);
			else 
				_sendRedirect(client, MYCEOC);
		}
		else
			_sendRedirect(client, MYCEOC); 
	}
	request->setState(SEND);
}
