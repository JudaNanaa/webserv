/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:55 by itahri            #+#    #+#             */
/*   Updated: 2024/11/24 19:36:09 by madamou          ###   ########.fr       */
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
	private:
		std::string _header;
		// 		 |  filename | content ...
		File *_currentFile;
		std::vector<File *> _files;
		int					_fileState;
		char *				_body;
		char *				_request;
		std::string 		_boundary;
		unsigned int		_lenBody;
		long long		_lenTotalBody;
		unsigned int		_lenRequest;
	    std::ofstream    	_uploadFile;


		void				checkFileHeader( File& file, std::string &header );
	public:
		RawBits(void);
		~RawBits();

		int 				checkBondaries( void  );
		const std::string&	getHeader(void) const; 
		const char *		getBody(void) const; 
		const char *		getRequest(void) const;
		File* getCurrentFile(void) const; 
		void 				setBondary(std::string str);
		void 				BuffToRaw(const char *buff, const int n);
		void 				appendBody(const char *str, const int n);
		void 				splitRequest(void);
		void 				printBody(void) const;
		long 				find(const char *str) const;
    	long        		findInBody(const char *str, size_t n = 0) const;
		void 				setHeader(std::string header);
    	char*       		substrBody(size_t pos, size_t n);
		const unsigned int&	getLenBody(void) const;
		const long long&	getLenTotalBody(void) const;
    	int         		compareInBody(char *s, size_t n);
    	void        		eraseInBody(size_t pos, size_t n);
		int					handleFileHeader();
		void				flushBuffer( long pos, long n = 0 );
		int					handleFileBody();
};

#endif
