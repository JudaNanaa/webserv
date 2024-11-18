/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:36:18 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/18 17:35:55 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/includes.hpp"
#include <cstddef>
#include <stdatomic.h>

#ifndef FILE_HPP
# define FILE_HPP

class File {
	private:
		char*						_content;
		char*						_rawContent;
		size_t						_lenFile;
		std::string					_contentDisposition;
		std::string					_name;
		std::string					_extension;
		std::string					_filename;
		std::string					_contentType;
		std::map<std::string, std::string>		_others;
		std::map<std::string, std::string>		_infos;
	public:
		File();
		~File();
		// File( const File& other );
		// File& operator=( const File& other );

		const std::string&	contentDisposition( void );
		const char*	content( void );
		const char*	rawContent( void );
		const std::string&	name( void );
		const std::string&	extension( void );
		const std::string&	filename( void );
		const std::string&	contentType( void );
		const size_t		&lenFile(void) const;
		
		// search a the value of key, throw an std::exception if not found
		std::string			get( std::string key );

		void	initFile( char *rawContent, size_t lenFile );
		void	content( char* newContent );
		void	contentDisposition( std::string newContentDisposition );
		void	name( std::string newName );
		void	filename( std::string newFilename );
		void	contentType( std::string newContentType );
		void	lenFile(size_t len);
		void	set( std::string key, std::string value );
		void	freeContent(void);
};

#endif