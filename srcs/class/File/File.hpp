/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:36:18 by ibaby             #+#    #+#             */
/*   Updated: 2024/11/19 00:57:41 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/includes.hpp"
#include <cstddef>

#ifndef FILE_HPP
# define FILE_HPP

class File {
	private:
		char*						_content;
		size_t						_lenFile;
		std::map<std::string, std::string>		_infos;
	public:
		File();
		~File();
		
		const char*	content( void );
		const size_t		&lenFile(void) const;
		
		// search a the value of key, throw an std::exception if not found
		std::string			get( std::string key );

		void	content( char* newContent );
		void	lenFile(size_t len);
		void	set( std::string key, std::string value );
		void	freeContent(void);
};

#endif