/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:16:52 by madamou           #+#    #+#             */
/*   Updated: 2024/11/07 20:55:16 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "includes.hpp"
#include "Client.hpp"
#include <map>

# define MAX_CLIENTS 100
# define MAX_EVENTS 1000
# define PORT 9999


struct Data ;

class Server {
	private:
		int _socket_fd;
		std::map<int, Client> _clientMap;
		void addToEpoll(int fd, uint32_t events);
		void removeClient(int fd);
		int waitFdsToBeReady(void);
	public:
		Server();
		
		~Server();

		void	init();
		// static void	handleClient( int clientFd );
		void addClientToMap(Client &client);
		bool ifClientInServer(int fd) const;
		Client &getClient(int fd);
		void removeClientInMap(int fd);
    	void addData(Data* data);
		int getSocketFd() const;
		int nbOfClient(void) const;
    Data *_data;
};

#endif
