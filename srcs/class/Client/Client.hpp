#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../RawBits/RawBits.hpp"
#include "../Request/Request.hpp"
// #include "Server.hpp"

class Server;

class Client {
	private:
		int _fd;
		int _pipeFD;
		int _pid;
		int _CGIStatus;
		bool _useBuffer;
		bool _readyToParseHeader;
		bool _readyToParseBody;
		bool _readyToResponse;
		Request *_request;
	public:
		Client(int const fd, Server *server);
		~Client();

		Client &operator=(Client const &other);
		Server *_server;
		
		void setPid(int pid);
		const int &getPid(void) const;
		void setCGIStatus(int pid);
		const int &getCGIStatus(void) const;
		void	cleanRequest( void );
		void	setCGIFD(int fd);
		int getCGIFD(void);
		void setUseBuffer(bool boolean);
		bool getUseBuffer(void) const;
		int getClientFd(void) const;
		void setReadyToresponse(bool boolean);
		bool const &isReadyToResponse(void) const; 
		void pushHeaderRequest(char *str, int n);
    	void setServerReq(Server *server);
		const t_state &whatToDo(void) const;
		const bool &getReadyToParseHeader(void) const;
		const bool &getReadyToParseBody(void) const;
		Request *getRequest(void);

};

#endif
