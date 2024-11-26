/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itahri <itahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:16:52 by madamou           #+#    #+#             */
/*   Updated: 2024/11/26 20:33:26 by itahri           ###   ########.fr       */
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

		//CGI
		void CgiDefaultGesture(Client *client);

		//request Parsing
		bool checkAllowMethodes(std::string methodes);
};

#endif
