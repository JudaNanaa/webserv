/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawBits.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itahri <itahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:56:55 by itahri            #+#    #+#             */
/*   Updated: 2024/11/05 14:57:46 by itahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAW_BITS_HPP
#define RAW_BITS_HPP

#include "../../../includes/includes.hpp"

class RawBits {
	private:
		std::string _header;
		char *_body;
		char *_request;
		unsigned int		_lenBody;
		unsigned int		_lenRequest;
		std::vector<char *> split(char *s, char *c);
	public:
		RawBits(void);
		~RawBits();

		const std::string &getHeader(void) const; 
		const char *getBody(void) const; 
		const char *getRequest(void) const; 
		// RawBits &operator+=(const char *str);
		void BuffToRaw(const char *buff, const int n);
		void appendBody(const char *str, const int n);
		void splitRequest(void);
		long find(const char *str) const;
		void setHeader(std::string header);
		const unsigned int &getLenBody(void) const;
		char *decode(void);
};

#endif