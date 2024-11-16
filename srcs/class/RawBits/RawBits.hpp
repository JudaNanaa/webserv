/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:55 by itahri            #+#    #+#             */
/*   Updated: 2024/11/15 19:00:56 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAW_BITS_HPP
#define RAW_BITS_HPP

#include "../../../includes/includes.hpp"
#include "../../../includes/utils.hpp"
#include "../File/File.hpp"
#include <vector>
#include <map>

class RawBits {
	private:
		std::string _header;
		// 		 |  filename | content ...
		std::vector<File> _files;
		char *_body;
		char *_request;
		char *_bondary;
		unsigned int		_lenBody;
		unsigned int		_lenRequest;

		void				checkFileHeader( std::string body, File& file );
	public:
		RawBits(void);
		~RawBits();

		// RawBits &operator+=(const char *str);
		void 				checkBondaries( void  );
		const std::string&	getHeader(void) const; 
		const char *		getBody(void) const; 
		const char *		getRequest(void) const;
    const std::vector<File>& getRawFile(void) const;
		void 				setBondary(char *str);
		void 				BuffToRaw(const char *buff, const int n);
		void 				appendBody(const char *str, const int n);
		void 				splitRequest(void);
		void 				printBody(void) const;
		long 				find(const char *str) const;
		void 				setHeader(std::string header);
		const unsigned int&	getLenBody(void) const;
};

#endif
