#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../RawBits/RawBits.hpp"
#include "../Request/Request.hpp"
// #include "Server.hpp"

class Server;

class Client {
	private:
		int _fd;
		bool _readyToParseHeader;
		bool _readyToParseBody;
		bool _readyToResponse;
		Request *_request;
	public:
		Client();
		Client(int const fd, Server *server);
		~Client();

		Client &operator=(Client const &other);
		Server *_server;
		int getClientFd(void) const;
		void setReadyToresponse(bool boolean);
		bool const &isReadyToResponse(void) const; 
		void pushHeaderRequest(char *str, int n);
		void pushBodyRequest(char *str, int n);
    	void setServerReq(Server *server);
		const t_state &whatToDo(void) const;
		const bool &getReadyToParseHeader(void) const;
		const bool &getReadyToParseBody(void) const;
		Request *getRequest(void);

};

#endif
