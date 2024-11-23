/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:55 by itahri            #+#    #+#             */
/*   Updated: 2024/11/23 17:45:59 by madamou          ###   ########.fr       */
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

# define FINISHED 1
# define NOT_FINISHED 0

# define STOP 1
# define CONTINUE 0

class RawBits {
	private:
		std::string _header;
		// 		 |  filename | content ...
		std::vector<File *> _files;
		int					_state;
		char *				_body;
		char *				_request;
		std::string 		_boundary;
		char *				_tmp;
		unsigned int		_lenBody;
		unsigned int		_lenRequest;

		void				checkFileHeader( File& file, std::string &header );
	public:
		RawBits(void);
		~RawBits();

		// RawBits &operator+=(const char *str);
		int 				checkBondaries( void  );
		void				deleteTmp(void);
		const std::string&	getHeader(void) const; 
		const char *		getBody(void) const; 
		const char *		getRequest(void) const;
    const std::vector<File*>& getRawFile(void) const;
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
    	int         		compareInBody(char *s, size_t n);
    	void        		eraseInBody(size_t pos, size_t n);
		void				cleanFiles(void);
		int					handleFileHeader( void );
		void				flushBuffer( long pos, long n = 0 );
		void				flushBuffer( std::string& buff );
};

#endif
