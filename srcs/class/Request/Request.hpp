/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:16:20 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/23 18:56:50 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../../../includes/includes.hpp"
#include "../RawBits/RawBits.hpp"
#include <map>
#include <ostream>
#include <string>

# define BUFFER_SIZE 1000000

# define DEFAULT_UPLOAD_FILE "URIs/uploads/default"

typedef enum s_parse
{
	NOT_READY,
	READY_PARSE_HEADER,
	READY_PARSE_BODY,
	ERROR,
}t_parse;

typedef enum s_state
{
	ON_HEADER,
	ON_BODY,
}t_state;

class Server;
class Client;

class Request : public RawBits {
	private:
		t_state _state;
		int	_method;
		bool _isCgi;
		std::string	_path;
		std::string	_Host;
		std::string _ResponsCode;
		std::map<std::string, std::string>	_others;
		Server *_server;
		long _contentLenght;
		Client *_client;
		bool _isRedirect;

		void	parseRequest(void);
		void	parseRequestLine( std::string line );

	public:
		Request(Client *client);
		~Request();

		/*	GETTER	*/
		int	method( void ) const;
		const std::string& path( void ) const;
		const std::string& host( void ) const;
		const std::string& find( std::string key ) const;
		RawBits *getRawRequest(void);
    	std::string getPath(void);
		const t_state &getStatus(void) const;
		const long	&getContentLenght(void) const;
		const int &getMethode(void) const;
		const std::string &getResponsCode(void) const;
		const std::vector<File*>& getFile(void) const;
		bool isACgi(void) const;
		std::string& getMap(std::string key);
		
		/*	SETTER	*/

		void	method( int newMethod );
		void	host( std::string newHost );
		void	setStatus( t_state newStatus );
		void	path( std::string newPath );
		void	add( std::string key, std::string value );
		void	setSizeBody(unsigned int nb);
		void setMethode(std::string methode);
		void setResponsCode(std::string code);
		void setIsACgi(bool boolean);
		t_parse addHeaderRequest(char *buff, int n);
		t_parse addBodyRequest(char *buff, int n, bool add);
    	void addServer(Server* server);
		void addRequestToMap(std::string key, std::string value);
		bool isKeyfindInHeader(std::string const &key) const;
		void	setRedirect(bool b);
		bool& 	getRedirect(void);

		/*	OTHERS	*/
		void	uploadBody(char *buff, int n, bool unusedBuffer);

		friend std::ostream& operator<<(std::ostream& os, const Request& request );
};

#endif
