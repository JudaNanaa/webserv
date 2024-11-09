/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:55:03 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/09 17:29:19 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "includes.hpp"
#include <ostream>

struct	Location {
	private:
		std::string		_location;

		std::string		_root;
		std::string		_cgi; // temporary a string maybe a special struc later
		std::string		_redirect;
		int				_allowedMethods;
		int				_client_max_body_size;

		std::string		_index;
		bool			_autoIndex;

		bool			_file_upload;
  		std::string		_uploadFolder;

	public:
		Location();
		~Location();
		// Location( const Location &other ); 	copy needed ?
		// Location& operator=( const Location &other );; 	copy needed ?

		/*	GETTER	*/
		const std::string&	location( void ) const;

		const std::string&	root( void ) const;
		const std::string&	cgi( void ) const;
		const std::string&	redirect( void ) const;
		int					allowedMethods( void ) const;
		int					maxBodySize( void ) const;

		const std::string&	index( void ) const;
		bool				autoIndex( void ) const;

		bool				fileUpload( void ) const;
		const std::string&	uploadFolder( void ) const;

		/*	SETTER	*/
		void	location( std::string newLocation );

		void	root( std::string newRoot );
		void	cgi( std::string newCgi );
		void	redirect( std::string newRedirect );
		void	allowedMethods( int newAllowedMethods );
		void	maxBodySize( int newMaxBodySize );

		void	index( std::string newIndex );
		void	autoIndex( bool newAutoIndex );
		
		void	fileUpload( bool newFileUpload );
		void	uploadFolder( std::string newUploadFolder );

};

std::string	isDefault(const std::string& var);

std::ostream& operator<<(std::ostream& os, const Location& location);
#endif