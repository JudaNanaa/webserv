/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:16:20 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/09 18:11:29 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "includes.hpp"
#include <map>
#include <ostream>
#include <string>

class Request {
	private:
		int	_method;
		std::string	_path;
		std::string	_Host;
		std::string	_Connection;
		std::string	_Accept;
		std::map<std::string, std::string>	_others;
	public:
		int	method( void ) const;
		const std::string& path( void ) const;
		const std::string& host( void ) const;
		const std::string& connection( void ) const;
		const std::string& accept( void ) const;
		const std::string& find( std::string key ) const;

		void	method( int newMethod );
		void	host( std::string newPath );
		void	connection( std::string newPath );
		void	accept( std::string newPath );
		void	add( std::string key, std::string value );
};

std::ostream& operator<<(std::ostream& os, const Request& request );

#endif