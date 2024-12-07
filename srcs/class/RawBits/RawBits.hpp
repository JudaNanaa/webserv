/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:55 by itahri            #+#    #+#             */
/*   Updated: 2024/12/07 20:20:46 by madamou          ###   ########.fr       */
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
	public:
		RawBits(void);
		~RawBits();

		int 				uploadMultipart( void  );
		const std::string&	getHeader(void) const; 
		const char *		getBody(void) const;
		void 				setBondary(const std::string &str);
		void 				BuffToRaw(const char *buff, const int n);
		void 				appendBody(const char *str, const int n);
		void 				splitRequest(void);
		long 				find(const char *str) const;
    	long        		findInBody(const char *str, size_t n = 0) const;
    	char*       		substrBody(size_t pos, size_t n);
		const unsigned int&	getLenBody(void) const;
		const long long&	getLenTotalBody(void) const;
    	void        		eraseInBody(size_t pos, size_t n);
		int					handleFileHeader();
		void				flushBuffer( long pos, long n = 0 );
		int					handleFileBody();
		const std::string &getBondary(void) const;		
};

#endif
