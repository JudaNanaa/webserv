/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itahri <itahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 14:23:02 by itahri            #+#    #+#             */
/*   Updated: 2024/11/24 14:30:33 by itahri           ###   ########.fr       */
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
  size_t extension = request->path().find_last_of('.');
  std::string cgi[2];
  
  cgi[0] = _data->_cgi[request->path().substr(extension)];
  cgi[1] = request->path().substr(extension);
}
