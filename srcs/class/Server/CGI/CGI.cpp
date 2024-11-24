/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtahri <mtahri@contact.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 14:23:02 by itahri            #+#    #+#             */
/*   Updated: 2024/11/24 14:54:56 by madtahri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <iomanip>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>
#include "../../../../includes/utils.hpp"
#include "../../RawBits/RawBits.hpp"
#include "../../Request/Request.hpp"
#include "../../GlobalData/GlobalData.hpp"
#include "../../Parser/Parser.hpp"

void Server::CgiDefaultGesture(Client *client) {
    Request* request = client->getRequest();
    std::size_t extension = request->path().find_last_of('.');
    char* cgi[3];

    cgi = n
    cgi[0] = _data->_cgi[request->path().substr(extension)].c_str();
    cgi[1] = (_data->_root + request->path()).c_str();
    cgi[2] = NULL;
  
    std::cerr << "cgi[0] : " << cgi[0] << std::endl;
    std::cerr << "cgi[1] : " << cgi[1] << std::endl;

    int pid = fork();
    execve(cgi[0], const_cast<char**>cgi);
}
