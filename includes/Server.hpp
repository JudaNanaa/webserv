/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:16:52 by madamou           #+#    #+#             */
/*   Updated: 2024/11/05 01:20:43 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "includes.hpp"

# define MAX_CLIENTS 100
# define MAX_EVENTS 1000
# define PORT 8080



class Server {
	private:
		int _socket_fd;
		struct sockaddr_in _server_addr;
		int _epoll_fd;
		struct epoll_event _events[MAX_EVENTS];
		void addToEpoll(int fd, uint32_t events);
		void addNewClient(void);
		void removeClient(int fd);
		int waitFdsToBeReady(void);
	public:
		Server();
		
		~Server();

		Server &operator=(Server const &other);

		void init();
		void run();
		void signalHandle();
};

#endif