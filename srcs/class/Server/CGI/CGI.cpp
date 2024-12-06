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
#include <unistd.h>
#include "../../../../includes/includes.hpp"
#include "../../RawBits/RawBits.hpp"
#include "../../Request/Request.hpp"
#include "../../GlobalData/GlobalData.hpp"
#include "../../Parser/Parser.hpp"

void Server::_childProcess(char **cgi, int ParentToCGI[2], int CGIToParent[2])
{
        close(ParentToCGI[1]);
        close(CGIToParent[0]);
        if (dup2(ParentToCGI[0], STDIN_FILENO) == -1)
        {
            close(CGIToParent[1]);
            std::cerr << "Error dup2 !" << std::endl;
            exit(1);
        }
        close(ParentToCGI[0]);
        if (dup2(CGIToParent[1], STDOUT_FILENO) == -1)
        {
            std::cerr << "Error dup2 !" << std::endl;
            exit(1);
        }
        close(CGIToParent[1]);
        alarm(5);
        execve(cgi[0], cgi, _env);
        std::cerr << "execve failed: ";
        perror(cgi[0]);
        exit(1);    
}

void closePipePanic(int pipe[2])
{
    if (pipe[0] != -1)
        close(pipe[0]);
    if (pipe[1] != -1)
        close(pipe[1]);
}

void Server::_setCgiArgs(Client *client, char **cgi)
{
    Request* request = client->getRequest();
    std::size_t extension = request->path().find_last_of('.');

    std::string tmp = _data->_root + request->path();
    std::cerr << "executable: " << _data->_cgi[request->path().substr(extension)] << " | path: " << (char *)const_cast<char*>((_data->_root + request->path()).c_str()) << std::endl;
    cgi[0] = const_cast<char*>(_data->_cgi[request->path().substr(extension)].c_str());
    cgi[1] =  const_cast<char*>(tmp.c_str());
    cgi[2] = NULL;
    std::cerr << "cgi[0] : " << cgi[0] << std::endl;
    std::cerr << "cgi[1] : " << cgi[1] << std::endl;
}

void Server::CgiDefaultGesture(Client *client) {
    
    char* cgi[3];
    int ParentToCGI[2] = {-1, -1};
    int CGIToParent[2] = {-1, -1};

    _setCgiArgs(client, cgi);
    if (pipe(ParentToCGI) == -1 || pipe(CGIToParent) == -1)
    {
        closePipePanic(ParentToCGI), closePipePanic(CGIToParent);
        std::cerr << "Pipe fail !" << std::endl;
        client->setResponse("500");
        return;
    }
    int pid = fork();
    if (pid == -1)
    {
        closePipePanic(ParentToCGI), closePipePanic(CGIToParent);
        std::cerr << "Fork fail !" << std::endl;
        client->setResponse("500");
        return;
    }
    if (pid == 0)
        _childProcess(cgi, ParentToCGI, CGIToParent);
    close(ParentToCGI[0]), close(CGIToParent[1]);
	client->setParentToCGI(ParentToCGI[1]);
    client->setCGIFD(CGIToParent[0]);
    client->setPid(pid);
}

void	Server::handleCgi( Client *client ) {
	client->getRequest()->setIsACgi(true);
	CgiDefaultGesture(client);
}
