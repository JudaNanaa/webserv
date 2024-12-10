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

void Server::_writeBodyToCgi(Client *client, const char *buff, int n)
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
	}
	else if (clientRequest->getLenTotalBody() > clientRequest->getContentLenght())
	{
		close(client->getParentToCGI());
		client->setParentToCGI(-1);
		client->setResponse("413");
	}
}

void	Server::_execChildProcess(char **cgi, int ParentToCGI[2], int CGIToParent[2])
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

void Server::_childProcess(Client *client , int ParentToCGI[2], int CGIToParent[2])
{
    char *cgi[3];
    Request* request = client->getRequest();
    std::size_t extension = request->path().find_last_of('.');
    std::string script_path;
    std::string interpreter_path;

    if (_isLocation(request->path())) {
        Location *location = _data->getLocation(request->path());
        std::map<std::string, std::string> cgi_map = location->cgi();
        interpreter_path = cgi_map.find(request->path().substr(extension))->second.c_str();
        script_path = location->root() + request->path();
    }
    else
    {
        script_path = _data->_root + request->path();
        interpreter_path = _data->_cgi[request->path().substr(extension)].c_str();
    }
    std::cerr << "executable: " << _data->_cgi[request->path().substr(extension)] << " | path: " << (char *)const_cast<char*>((_data->_root + request->path()).c_str()) << std::endl;
    cgi[0] = const_cast<char*>(interpreter_path.c_str());
    cgi[1] =  const_cast<char*>(script_path.c_str());
    cgi[2] = NULL;
    std::cerr << "cgi[0] : " << cgi[0] << std::endl;
    std::cerr << "cgi[1] : " << cgi[1] << std::endl;

	_execChildProcess(cgi, ParentToCGI, CGIToParent);
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

		if (pid is -1)	// no 
			continue ;
		else {
			int	status;
			switch (waitpid(pid, &status, WNOHANG)) {
				case -1: { std::cerr << "waitpid failed" << std::endl ; continue; } // error 
				case 0: { continue; } // not finished
				default:
					printnl("CGI FINISHED !");
					client->setResponse();
					client->setCGIStatus(status);
			}
		}
	}
}

void	Server::_handleCGI( Client *client ) {
    int ParentToCGI[2] = {-1, -1};
    int CGIToParent[2] = {-1, -1};

	std::cerr << "CGI" << std::endl;
	client->getRequest()->setRequestType(CGI);
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
	if (client->getRequest()->getContentLenght() == -1)
		close(ParentToCGI[1]);
	else
		client->setParentToCGI(ParentToCGI[1]);
    client->setCGIFD(CGIToParent[0]);
    client->setPid(pid);
}
