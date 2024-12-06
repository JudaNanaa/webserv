/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:16:52 by madamou           #+#    #+#             */
/*   Updated: 2024/12/06 19:00:50 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "../../../includes/includes.hpp"
#include "../Client/Client.hpp"
#include "../Request/Request.hpp"
#include "../Parser/Parser.hpp"
#include "../Parser/Location.hpp"
#include <exception>
#include <map>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>

# define MAX_CLIENTS 1000
# define MAX_EVENTS 1000

struct Data ;

class Server {
	private:
		int _socket_fd;
		std::string _host[2];
		std::map<int, Client*> _clientMap;
		char	**_env;

		// CGI
		void	_responseCgiIfNoProblem(Client *client);
		void	_responseCgiError(Client *client);
		void	_childProcess(Client *client, int ParentToCGI[2], int CGIToParent[2]);

		// Parsing header
		void	_parseClientHeader(Client *client);
		void	_addHeaderLine( std::string line, Request *clientRequest);
		void	_addingHeader(Client *client, char *buff, int n);
		void	_parseOtherLinesHeader(Client *client, const std::vector<std::string> &headerSplit);
		void	_parseContentLengthAndBoundary(Request *clientRequest);
		void	_parseFirstLineHeader(Client *client, const std::vector<std::string> &headerSplit);
		
		// Response
		std::string  generateAutoIndex(Client *client, const std::string &directoryPath);
		int	sendToFd(const char *msg, std::size_t msgSize, int fd);
		std::string _openResponseFile(Request *clientRequest);
		std::string _normalOpenFile(Request *clientRequest);
		
		std::string	getContentType(const std::string& path);
		std::string	getResponseHeader(Request *request, const std::string& path);
	public:
		Server();
		
		~Server();

		void checkCgi( void );

		void	setEnv( char **env );
		char	**	getEnv( void ) const;
		void	init();
		void checkBoundaries( void );
		void addClientToMap(Client *client);
		bool ifClientInServer(int fd) const;
		Client *getClient(int fd);
		void removeClientInMap(int fd);
  		void addData(Data* data);
		int getSocketFd() const;
		bool isServerHost(std::string const &str) const;
		void addClientRequest(int fd);
		void responseCGI(Client *client);
		void sendResponse(int fd, Client *client);
		void sendRedirect(std::string redirect, int fd, Client *client);
		void giveClientResponse(int fd);
    	void giveClientResponseByLocation(int fd);
    	void MergeLocationData(std::string path);
		Location *findLocation(const std::string &uri);
		void chooseParsing( Client *client );
    	Data *_data;
		bool	isCgi( const std::string& path );
		void	handleCgi( Client *client );
		void	handleLocation(Client *client);
		void	handleRequest( Client *client );
		void 	handleAuth(Client* client);
		void	handleDELETE(Client* client);
		void 	writeBodyToCgi(Client *client, char *buff, int n);
		//CGI

		//request Parsing
		bool checkAllowMethodes(std::string methodes);
};

#endif
