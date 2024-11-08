#ifndef GLOBALDATA_HPP
#define GLOBALDATA_HPP

#include "Server.hpp"
#include "includes.hpp"
#include "utils.hpp"
#include <exception>
#include <vector>
#include <map>

class GlobalData {
	private:
		std::map<int, Server> _servMap;
		int _epoll_fd;
		struct epoll_event _events[MAX_EVENTS];
		void addToEpoll(int fd, uint32_t events);
		int waitFdsToBeReady(void);
		Client &searchClient(const int fd);
		void addNewClient(Server &server);
		void initServers(std::vector<Server> &servVec);
		void handleClientIn(int fd);
		void handleClientOut(int fd);
		void removeClient(int fd);
		bool isServerFd(const int &fd) const;
	public:
		GlobalData();

		~GlobalData();

		void runServers(std::vector<Server> &servVec);
		void closeServers(void);
};

#endif