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

#include "../../../includes/includes.hpp"
#include "../RawBits/RawBits.hpp"
#include <map>
#include <ostream>
#include <string>

# define BUFFER_SIZE 4096

typedef enum s_parse
{
	NOT_READY,
	READY_PARSE_HEADER,
	READY_PARSE_BODY,
}t_parse;

typedef enum s_state
{
	ON_HEADER,
	ON_BODY,
}t_state;

class Server;

class Request {
	private:
		t_state _state;
		RawBits *_request;
		int	_method;
		std::string	_path;
		std::string	_Host;
		std::map<std::string, std::string>	_others;
		Server *_server;
		unsigned int _contentLenght;

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
		RawBits *getRawRequest(void);
    	std::string getPath(void);
		const t_state &getStatus(void) const;
		
		/*	SETTER	*/

		void	method( int newMethod );
		void	host( std::string newHost );
		void	path( std::string newPath );
		void	add( std::string key, std::string value );
		void	setSizeBody(unsigned int nb);
		
		t_parse addHeaderRequest(char *buff, int n);
		t_parse addBodyRequest(char *buff, int n);
    	void addServer(Server* server);
		void addRequestToMap(std::string key, std::string value);
		bool isKeyfindInHeader(std::string const &key) const;
		
		friend std::ostream& operator<<(std::ostream& os, const Request& request );
};

#endif
