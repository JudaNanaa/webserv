/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:18:07 by madamou           #+#    #+#             */
/*   Updated: 2024/12/30 22:34:08 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#define GET_ 00000001
#define POST_ 00000010
#define DELETE_ 00000100
#define OPTIONS_ 00001000

typedef enum s_state
{
	ON_HEADER,
	READY_PARSE_HEADER,
	ON_BODY,
	RESPONSE,
	SEND,
	ERROR,
}t_state;

# define BUFFER_SIZE 8192

# define DEFAULT_UPLOAD_FILE "default"

# define POST_HTML "serverDefaultPages/post.html"
# define DELETE_HTML "serverDefaultPages/delete.html"
# define ERROR_FOLDER "serverDefaultPages/errors/"
# define DEFAULT_CONFIG_PATH "serverDefaultPages/configExample.conf"

#define SECRET "https://www.cat29.fr/riche-et-independant/"
#define SINGE "https://media.tenor.com/_uIJwdpxI8UAAAAM/mono-serio.gif"
#define MYCEOO "https://pbs.twimg.com/media/EaoNasxXYAUE8pA.jpg"
#define MYCEOC "https://pbs.twimg.com/media/EaoNYXkXkAMHJJK.jpg"

# define DEFAULT 1
# define LOCATION 3

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fcntl.h>
#include <iomanip>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>

# define printnl(arg) std::cerr << arg << std::endl
# define is ==
# define is_not !=
# define not_found == std::string::npos
# define is_found != std::string::npos

extern bool g_running;
extern char g_buffer[BUFFER_SIZE];

#endif