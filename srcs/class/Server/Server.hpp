/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:16:52 by madamou           #+#    #+#             */
/*   Updated: 2024/12/02 20:05:41 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "../../../includes/includes.hpp"
#include "../Client/Client.hpp"
#include "../Request/Request.hpp"
#include "../Parser/Parser.hpp"
#include <exception>
#include <map>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>

# define MAX_CLIENTS 100
# define MAX_EVENTS 1000
# define PORT 9999

struct Data ;

class Server {
	private:
		int _socket_fd;
		std::string _host[2];
		std::map<int, Client*> _clientMap;
		char	**_env;

		void _parseClientHeader(Client *client);
		void	_parseRequestLine( std::string line, Request *clientRequest);
		void _responseCgiIfNoProblem(Client *client);
		void _responseCgiError(Client *client);
		std::string	getContentType(const std::string& path);
		std::string	getResponseHeader(Request *request, const std::string& path, std::size_t fileSize);
		int	sendToFd(const char *msg, std::size_t msgSize, int fd);

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
		int nbOfClient(void) const;
		void addClientRequest(int fd);
		void responseCGI(Client *client);
		void sendResponse(int fd, Client *client);
		void sendRedirect(std::string redirect, int fd, Client *client);
		void giveClientResponse(int fd);
    	void giveClientResponseByLocation(int fd);
    	void MergeLocationData(std::string path);
		Location *findLocation(const std::string &uri);
		void chooseParsing( Client *client );
		void parseHeaderWithLocation(Client *client, Request *request);
    	Data *_data;
		bool	isCgi( const std::string& path );
		void	handleCgi( Client *client );
		void	handleLocation(Client *client);
		void	handleRequest( Client *client );
		void 	handleAuth(Client* client);
		void	handleDELETE(Client* client);
		void 	writeBodyToCgi(Client *client, char *buff, int n);
		//CGI
		void	CgiDefaultGesture(Client *client);

		//request Parsing
		bool checkAllowMethodes(std::string methodes);
};

#endif
