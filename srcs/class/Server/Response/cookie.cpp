/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:25:20 by madamou           #+#    #+#             */
/*   Updated: 2024/12/14 16:26:18 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

#define SECRET "https://www.cat29.fr/riche-et-independant/"
#define SINGE "https://media.tenor.com/_uIJwdpxI8UAAAAM/mono-serio.gif"
#define MYCEOO "https://pbs.twimg.com/media/EaoNasxXYAUE8pA.jpg"
#define MYCEOC "https://pbs.twimg.com/media/EaoNYXkXkAMHJJK.jpg"

void setCookie(std::stringstream &response, std::string key, std::string value) {
	response
	<< "Set-Cookie: " << key << "=" << value << ";"
	<< "Max-Age=300"
	<< "\r\n";
}

void Server::_handleAuth(Client* client) {
	Request* request = client->getRequest();
	std::stringstream response;

	client->getRequest()->setResponsCode("302");
	if (request->path() is "/auth/login"){
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
		std::string header = request->getHeader();
		if (request->isKeyfindInHeader("Cookie") is true) {
			if (request->find("Cookie").find("auth=true") is_found) 
				_sendRedirect(SECRET, client->getClientFd(), client);
			else 
				_sendRedirect(MYCEOC, client->getClientFd(), client);
		}
		else
			_sendRedirect(MYCEOC, client->getClientFd(), client); 
	}
	request->setState(SEND);
}
