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

#include "includes.hpp"

class RawBits {
	private:
		char*	_content;
		int		_len;
		std::vector<char *> split(char *s, char *c);
	public:
		RawBits(void);
		~RawBits();
		void pushBack(unsigned char c);
		char *decode(void);
};

#endif