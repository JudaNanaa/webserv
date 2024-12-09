/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:55 by itahri            #+#    #+#             */
/*   Updated: 2024/12/10 23:56:20 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:55 by itahri            #+#    #+#             */
/*   Updated: 2024/11/23 17:43:42 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAW_BITS_HPP
#define RAW_BITS_HPP

#include "../../../includes/includes.hpp"
#include "../../../includes/utils.hpp"
#include "../File/File.hpp"
#include <string>
#include <vector>
#include <map>
#include <fstream>

# define FINISHED 2
# define NOT_FINISHED 3

# define STOP 1
# define CONTINUE 0

class RawBits {
	protected:
		std::string			_header;
		File				_currentFile;
		int					_fileState;
		char *				_body;
		char *				_request;
		std::string 		_boundary;
		unsigned int		_lenBody;
		long long			_lenTotalBody;
		unsigned int		_lenRequest;
		std::ofstream		_uploadFile;

		void				checkFileHeader( File& file, std::string &header );
		void 				_appendBody(const char *str, const int n);
		long 				_findInRequest(const char *str) const;
    	char*       		_substrBody(size_t pos, size_t n);
		void				_flushBuffer( long pos, long n = 0 );

	public:
		RawBits(void);
		~RawBits();
		
		// Getter
		const std::string	&getHeader(void) const; 
		const char			*getBody(void) const;
		const unsigned int	&getLenBody(void) const;
		const long long		&getLenTotalBody(void) const;
		const std::string	&getBondary(void) const;
		
		// Setter
		void 				setBondary(const std::string &str);
		
		
		int 				uploadMultipart( void );
		void 				BuffToRequest(const char *buff, const int n);
		void 				splitRequest(void);
    	long        		findInBody(const char *str, size_t n = 0) const;
    	void        		eraseInBody(size_t pos, size_t n);
		int					handleFileHeader();
		int					handleFileBody();
};

#endif
