/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:16:52 by madamou           #+#    #+#             */
/*   Updated: 2024/12/10 18:309:40 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../RawBits/RawBits.hpp"
#include "../Request/Request.hpp"
#include "../GlobalData/GlobalData.hpp"
#include "../Parser/Parser.hpp"
#include "../../../includes/includes.hpp"
#include "../Client/Client.hpp"
#include "../Request/Request.hpp"
#include "../Parser/Parser.hpp"
#include "../Parser/Location.hpp"

# define MAX_CLIENTS 1000

struct Data ;

class Server {
	private:
		int						_socket_fd;
		std::string				_host[2];
		std::map<int, Client*>	_clientMap;
		char					**_env;

		// CGI
		void					_responseCgiIfNoProblem(Client *client);
		void					_responseCgiError(Client *client);
		void					_childProcess(Client *client, int ParentToCGI[2], int CGIToParent[2]);
		void					_responseCGI(Client *client);
		void					_handleCGI( Client *client );
		void					_writeBodyToCgi(Client *client, const char *buff, int n);
		void					_execChildProcess(char **cgi, int ParentToCGI[2], int CGIToParent[2]);
		int						_checkCgi(Client *client);
		// Parsing header
		void					_parseClientHeader(Client *client);
		void					_addHeaderLine( std::string line, Request *clientRequest);
		void					_addingHeader(Client *client, const char *buff, const int &n);
		void					_parseOtherLinesHeader(Client *client, const std::vector<std::string> &headerSplit);
		void					_parseContentLengthAndBoundary(Request *clientRequest);
		void					_parseFirstLineHeader(Client *client, const std::vector<std::string> &headerSplit);
		void					_chooseParsing( Client *client );
		void					_handleRequest( Client *client );
		
		// Response
		std::string				_generateAutoIndex(Client *client, const std::string &directoryPath);
		std::string				_openResponseFile(Request *clientRequest, Client* client);
		std::string				_normalOpenFile(Request *clientRequest, Client* client);
		std::string				_getContentType(const std::string& path);
		std::string				_getResponseHeader(Request *request, const std::string& path);
		int						_sendToFd(const char *msg, std::size_t msgSize, int fd);
		void					_sendResponse(int fd, Client *client);
		void					_sendRedirect(std::string redirect, int fd, Client *client);
		void					_sendResponseDefault(Client *client);
		// Location
		void					_sendResponseLocation(Client *client);
		void					_handleLocation(Client *client);
		bool					_checkLocationCgi(Location* location, std::string extension, Client* client);
		bool					_isLocation(const std::string &path);
		// Getter
		Client					*_getClient(int fd);

		// Auth
		void					_handleAuth(Client* client);
		
		// Delete
		void					_handleDelete(Client* client);

		void					_addingBody(Client *client, const char *buff, const int &n);
	public:
		Server();
		
		~Server();

    	Data					*_data;
		/*		SETTER		*/
		void					setEnv(char **env);
		bool					isCgi(const std::string& path);
  		void					addData(Data* data);
		
		/*		GETTER		*/
		char					**getEnv( void ) const;
		int const				&getSocketFd() const;
		bool					ifClientInServer(int fd) const;
		bool					isServerHost(std::string const &str) const;

		// Other 
		void					init();
		void					addClientToMap(Client *client);
		void					removeClientInMap(int fd);
		t_state					addClientRequest(int fd);
		t_state					giveClientResponse(int fd);
		void					freeAll(void);
};

#endif
