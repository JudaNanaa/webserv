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
#include <cstddef>
#include <cstring>
#include <unistd.h>

void Server::_writeBodyToCgi(Client *client, const char *buff, const std::size_t &n)
{
	Request *clientRequest = client->getRequest();

	if (client->getUseBuffer() is true)
	{
		write(client->getParentToCGI(), buff, n);
		clientRequest->incrementSizeBody(n);
	}
	else
		write(client->getParentToCGI(), clientRequest->getBody(), clientRequest->getLenBody());
	if (clientRequest->getLenTotalBody() is clientRequest->getContentLenght())
	{
		close(client->getParentToCGI());
		client->setParentToCGI(-1);
		client->setResponse("200");
	}
	else if (clientRequest->getLenTotalBody() > clientRequest->getContentLenght())
	{
		close(client->getParentToCGI());
		client->setParentToCGI(-1);
		client->setResponse("413");
	}
}

void	Server::_execChildProcess(char **cgi, const int ParentToCGI[2], const int CGIToParent[2])
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

int Server::_checkCgiArgsPath(Request *request, const std::string &interpreter_path, const std::string &script_path)
{
	if (access(interpreter_path.c_str(), F_OK | X_OK) == -1)
	{
		printnl("Executable not found: 502 Error");
		request->setResponsCode("502");
		return -1;
	}
	else if (access(script_path.c_str(), F_OK) == -1)
	{
		printnl("CGI file not found: 404 Error");
		request->setResponsCode("404");
		return -1;
	}
	else if (access(script_path.c_str(), R_OK) == -1)
	{
		printnl("CGI file not found: 404 Error");
		request->setResponsCode("403");
		return -1;
	}
	return 0;
}

int Server::_setCgiArgs(Request *request, char **cgi)
{
    std::size_t extension = request->path().find_last_of('.');
    std::string script_path;
    std::string interpreter_path;

    if (_isLocation(request->path())) {
        Location *location = _data->getLocation(request->path());
        std::map<std::string, std::string> cgi_map = location->cgi();
        interpreter_path = cgi_map.find(&request->path()[extension])->second;
        if (location->root().empty())
        	script_path = _data->_root + request->path();
        else 
        	script_path =  location->root() + &request->path()[location->location().size()];
    }
    else
    {
        script_path = _data->_root + request->path();
        interpreter_path = _data->_cgi[&request->path()[extension]];
    }
	if (_checkCgiArgsPath(request, interpreter_path, script_path) == -1)
		return -1;
	cgi[0] = new char[interpreter_path.length() + 1];
	std::memcpy(cgi[0], interpreter_path.data(), interpreter_path.length() + 1);
	cgi[1] = new char[script_path.length() + 1];
	std::memcpy(cgi[1], script_path.data(), script_path.length() + 1);
    cgi[2] = NULL;
	return 0;
}

void closePipePanic(const int pipe[2])
{
    if (pipe[0] != -1)
        close(pipe[0]);
    if (pipe[1] != -1)
        close(pipe[1]);
}

void	Server::_handleCGI( Client *client ) {
	char *cgi[3];
    int ParentToCGI[2] = {-1, -1};
    int CGIToParent[2] = {-1, -1};

	std::cerr << "CGI" << std::endl;
	client->getRequest()->setRequestType(CGI);
	if (_setCgiArgs(client->getRequest(), cgi) == -1)
		return;
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
		_execChildProcess(cgi, ParentToCGI, CGIToParent);
    close(ParentToCGI[0]), close(CGIToParent[1]);
	delete [] cgi[0], delete [] cgi[1];
	if (client->getRequest()->getContentLenght() == -1)
	{
		close(ParentToCGI[1]);
		client->getRequest()->setState(RESPONSE);
	}
	else
		client->setParentToCGI(ParentToCGI[1]);
    client->setCGIFD(CGIToParent[0]);
    client->setPid(pid);
}

int Server::_checkCgi(Client *client) {
	int	status;
	int returnValue;
	int pid = client->getPid();
	
	if (pid == -1)
		return FINISHED;
	returnValue = waitpid(pid, &status, WNOHANG);
	if (returnValue == -1)
	{
		std::cerr << "waitpid failed (pid == " << pid << ")" << std::endl;
		client->setResponse("500");
	}
	else if (returnValue == 0)
		return NOT_FINISHED;
	else
	{
		printnl("CGI FINISHED !");
		client->setCGIStatus(status);
	}
	return FINISHED;
}
