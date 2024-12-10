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

		void					_addToEpoll(int fd, uint32_t events);
		int						_waitFdsToBeReady(void);
		void					_addNewClient(Server &server);
		void					_initServers(std::vector<Server> &servVec);
		void					_handleClientIn(int fd);
		void					_handleClientOut(int fd);
		void					_removeClient(int fd);
		bool					_isServerFd(const int &fd);
		Server 					*_getServerWithClientFd(const int fd);
		void					_handleEvent( struct epoll_event& fdsReady );
	public:
		GlobalData();

		~GlobalData();

		void					runServers(std::vector<Server> &servVec);
		void					closeServers(void);
};

#endif
