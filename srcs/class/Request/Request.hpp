/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:16:20 by ibaby             #+#    #+#             */
/*   Updated: 2024/12/12 17:20:18 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../../../includes/includes.hpp"
#include "../RawBits/RawBits.hpp"
#include "../Parser/Location.hpp"
#include <cstddef>
#include <fstream>
#include <map>
#include <ostream>
#include <string>

class Server;
class Client;

typedef struct s_responseFile
{
	std::ifstream file;
	std::size_t fileSize;
	std::size_t totalSend;
} t_responseFile;

class Request : public RawBits {
	private:
		t_state								_state;
		int									_requestType;
		int									_method;
		std::ofstream						_defaultFile;
		std::string							_path;
		std::string							_ResponsCode;
		std::map<std::string, std::string>	_others;
		long long							_contentLenght;
		Server								*_server;
		Client								*_client;
		bool								_isRedirect;
		t_responseFile						_responseFile;
		Location							*_location;

		// Parsing Header
		void								_parseRequest(void);
		void								_parseRequestLine( std::string line );

		// Upload
		void								_uploadBody(void);

	public:
		Request(Client *client, Server *server);
		~Request();

		/*	GETTER	*/
		const int							&method( void ) const;
		const std::string					&path( void ) const;
		const std::string					&find( std::string key ) const;
		const t_state						&getState(void) const;
		const long long						&getContentLenght(void) const;
		const std::string					&getResponsCode(void) const;
		bool								responseFileOpen() const;
		std::size_t							getResponseFileSize(void) const;
		std::size_t							getResponseFileTotalSend(void) const;
		const int							&getRequestType(void) const;
		bool								isKeyfindInHeader(std::string const &key) const;
		bool								getRedirect(void) const;
		Location							*getLocation(void) const;
		
		/*	SETTER	*/
		void								setRequestType(const int &type);
		void								addResponseFileTotalSend(std::size_t nbSend);
		void								method( const int& newMethod );
		void								setState( t_state newStatus );
		void								path( std::string newPath );
		void								setSizeBody(long long nb);
		void								setMethode(const std::string &methode);
		void								setResponsCode(const std::string &code);
		void								setRedirect(const bool &b);
		void								incrementSizeBody(const unsigned long long &n);
		void								setLocation(Location *location);
		// Parsing Header
		void								addHeaderLineToMap(const std::string &key, const std::string &value);
		void								addHeaderRequest(const char *buff, const int &n);
		void								addBodyRequest(const char *buff, const int &n, const bool &add);

		// Body
		void								openResponseFile(const char *fileName);
		/*	Response file	*/
		std::size_t							readResponseFile(char *buffer, std::size_t n);
		void								closeResponseFile(void);
		
		friend std::ostream& operator<<(std::ostream& os, const Request& request );
};

#endif
