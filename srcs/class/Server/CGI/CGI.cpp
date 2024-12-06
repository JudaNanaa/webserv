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
#include <map>
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
#include <sys/wait.h>
#include "../../../../includes/includes.hpp"
#include "../../RawBits/RawBits.hpp"
#include "../../Request/Request.hpp"
#include "../../GlobalData/GlobalData.hpp"
#include "../../Parser/Parser.hpp"

void Server::writeBodyToCgi(Client *client, char *buff, int n)
{
	Request *clientRequest = client->getRequest();
	int result;

	if (client->getUseBuffer() is true)
	{
		result = write(client->getParentToCGI(), buff, n);
		clientRequest->incrementSizeBody(n);
	}
	else
		result = write(client->getParentToCGI(), clientRequest->getBody(), clientRequest->getLenBody());
	(void)result;
	if (clientRequest->getLenTotalBody() is clientRequest->getContentLenght() || clientRequest->getContentLenght() is -1)
	{
		close(client->getParentToCGI());
		client->setParentToCGI(-1);
	}
}

// void printMap(std::map<std::string, std::string> map)
// {
//     std::map<std::string, std::string>::iterator it = map.begin();
//     std::map<std::string, std::string>::iterator end = map.end();

//     while (it is_not end)
//     {
//         printnl("key == " + it->first << " value == " + it->second);
//         it++;
//     }
// }

void Server::_childProcess(Client *client , int ParentToCGI[2], int CGIToParent[2])
{
    char *cgi[3];
    Request* request = client->getRequest();
    std::size_t extension = request->path().find_last_of('.');

    std::string script_path;
    std::string interpreter_path;
    if (_data->checkLocation(request->path())) {
        Location *location = _data->checkLocation(request->path());
        std::map<std::string, std::string> cgi_map = location->cgi();
        interpreter_path = cgi_map.find(request->path().substr(extension))->second.c_str();
        script_path = location->root() + request->path();
    }
    else
    {
        script_path = _data->_root + request->path();
        interpreter_path = _data->_cgi[request->path().substr(extension)].c_str();
        std::cerr << "debug : " << script_path << std::endl;
    }

    std::cerr << "executable: " << _data->_cgi[request->path().substr(extension)] << " | path: " << (char *)const_cast<char*>((_data->_root + request->path()).c_str()) << std::endl;
    cgi[0] = const_cast<char*>(interpreter_path.c_str());
    cgi[1] =  const_cast<char*>(script_path.c_str());
    cgi[2] = NULL;
    std::cerr << "cgi[0] : " << cgi[0] << std::endl;
    std::cerr << "cgi[1] : " << cgi[1] << std::endl;

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

void Server::checkCgi( void ) {
	std::map<int, Client*>::iterator it, ite;

	it = _clientMap.begin();
	ite = _clientMap.end();

	for (;it != ite; it++) {
		Client	*client = it->second;
		int		pid = client->getPid();

		if (pid is -1) {	// no 
			continue ;
		} else {
			int	status;
			switch (waitpid(pid, &status, WNOHANG)) {
				case -1: { std::cerr << "waitpid failed" << std::endl ; continue; } // error 
				case 0: { continue; } // not finished
				default:
					client->setResponse();
					client->setCGIStatus(status);
			}
		}
	}
}

void	Server::handleCgi( Client *client ) {
    int ParentToCGI[2] = {-1, -1};
    int CGIToParent[2] = {-1, -1};

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
        _childProcess(client, ParentToCGI, CGIToParent);
    close(ParentToCGI[0]), close(CGIToParent[1]);
	client->setParentToCGI(ParentToCGI[1]);
    client->setCGIFD(CGIToParent[0]);
    client->setPid(pid);
}
