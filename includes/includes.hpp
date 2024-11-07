/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:18:07 by madamou           #+#    #+#             */
/*   Updated: 2024/11/05 00:32:19 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

class ErrorException : public std::exception
{
	std::string _msg;
	public:

		ErrorException(const std::string& str)
			: _msg(std::string("Empty Image : ") + str)
		{

		}

	virtual const char* what() const throw()
	{
		return _msg.c_str();
	}
};

extern bool g_running;

#endif