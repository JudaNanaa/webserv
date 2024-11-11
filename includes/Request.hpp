/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:16:20 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/10 18:33:13 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "includes.hpp"
#include <map>
#include <ostream>
#include <string>

# define BUFFER_SIZE 4096

class Server;

class Request {
	private:
		int	_method;
		std::string	_path;
		std::string	_Host;
		std::map<std::string, std::string>	_others;
		std::string	_request;
		Server *_server;

		void	parseRequest(void);
		void	parseRequestLine( std::string line );

	public:
		Request();
		~Request();

		/*	GETTER	*/
		int	method( void ) const;
		const std::string& path( void ) const;
		const std::string& host( void ) const;
		const std::string& find( std::string key ) const;
		
		/*	SETTER	*/

		void	method( int newMethod );
		void	host( std::string newHost );
		void	path( std::string newPath );
		void	add( std::string key, std::string value );
		
		void addRequest(std::string str);
    void addServer(Server* server);
		
		friend std::ostream& operator<<(std::ostream& os, const Request& request );
};

#endif
