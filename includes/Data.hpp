#ifndef DATA_HPP
#define DATA_HPP

#include "Server.hpp"
#include "includes.hpp"
#include "utils.hpp"
#include <vector>

class Data {
	private:
		std::vector<Server> servVec;
		int _epoll_fd;
		int initEpoll(void);
		void addToEpoll(int fd, uint32_t events);
	public:
		Data();

		~Data();

		void initServers(void);
		void runServers(void);
		void closeServers(void);
};

#endif