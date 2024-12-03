/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:16:20 by ibaby             #+#    #+#             */
/*   Updated: 2024/12/03 20:08:42 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../../../includes/includes.hpp"
#include "../RawBits/RawBits.hpp"
#include <cstddef>
#include <fstream>
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

typedef struct s_responseFile
{
	std::ifstream file;
	std::size_t fileSize;
	std::size_t totalSend;
} t_responseFile;

class Request : public RawBits {
	private:
		t_state _state;
		int	_method;
		std::ofstream	defaultFile;
		bool _isCgi;
		std::string	_path;
		std::string	_Host;
		std::string _ResponsCode;
		std::map<std::string, std::string>	_others;
		Server *_server;
		long long _contentLenght;
		Client *_client;
		bool _isRedirect;
		t_responseFile _responseFile;

		void	parseRequest(void);
		void	parseRequestLine( std::string line );

	public:
		Request(Client *client);
		~Request();

		/*	GETTER	*/
		const int&	method( void ) const;
		const std::string& path( void ) const;
		const std::string& host( void ) const;
		const std::string& find( std::string key ) const;
		RawBits *getRawRequest(void);
    	std::string getPath(void);
		const t_state &getStatus(void) const;
		const long long	&getContentLenght(void) const;
		const int &getMethode(void) const;
		const std::string &getResponsCode(void) const;
		bool isACgi(void) const;
		std::string& getMap(const std::string &key);
		bool responseFileOpen() const;
		void	openResponseFile(const char *fileName);
		std::size_t getResponseFileSize(void) const;
		std::size_t getResponseFileTotalSend(void) const;
		
		/*	SETTER	*/

		void	addResponseFileTotalSend(std::size_t nbSend);
		void	method( const int& newMethod );
		void	host( std::string newHost );
		void	setStatus( t_state newStatus );
		void	path( std::string newPath );
		void	setSizeBody(long long nb);
		void setMethode(const std::string &methode);
		void setResponsCode(const std::string &code);
		void setIsACgi(bool boolean);
		t_parse addHeaderRequest(const char *buff, const int &n);
		t_parse addBodyRequest(const char *buff, const int &n, const bool &add);
    	void addServer(Server* server);
		void addHeaderLineToMap(const std::string &key, const std::string &value);
		bool isKeyfindInHeader(std::string const &key) const;
		void	setRedirect(const bool &b);
		bool& 	getRedirect(void);
		void	incrementSizeBody(const unsigned long long &n);

		/*	OTHERS	*/
		void	uploadBody();
		std::size_t	readResponseFile(char *buffer, std::size_t n);
		void	closeResponseFile(void);
		
		friend std::ostream& operator<<(std::ostream& os, const Request& request );
};

#endif
