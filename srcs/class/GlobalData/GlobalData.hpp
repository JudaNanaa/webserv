#ifndef GLOBALDATA_HPP
#define GLOBALDATA_HPP

#include "../Server/Server.hpp"
#include "../../../includes/includes.hpp"
#include "../../../includes/utils.hpp"
#include <exception>
#include <sys/epoll.h>
#include <vector>
#include <map>

# define MAX_EVENTS 1000

class GlobalData {
	private:
		std::map<int, Server>	_servMap;
		int						_epoll_fd;
		struct epoll_event		_events[MAX_EVENTS];

		void					_addToEpoll(const int &fd, const uint32_t &events);
		int						_waitFdsToBeReady(void);
		void					_addNewClient(Server &server);
		void					_initServers(std::vector<Server> &servVec);
		void					_handleClientIn(const int &fd);
		void					_handleClientOut(const int &fd);
		void					_removeClient(const int &fd);
		bool					_isServerFd(const int &fd);
		Server 					*_getServerWithClientFd(const int &fd);
		void					_handleEvent(const struct epoll_event& event);
		void					_modifyClientEvent(const int &fd, const uint32_t &events);
	public:
		GlobalData();

		~GlobalData();

		void					runServers(std::vector<Server> &servVec);
		void					closeServers(void);
};

#endif
