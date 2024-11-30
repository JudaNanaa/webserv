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
		int _ParentToCGI;
		int _pid;
		int _CGIStatus;
		bool _useBuffer;
		bool _readyToParseHeader;
		bool _readyToParseBody;
		bool _readyToResponse;
		Request *_request;
		void setReadyToresponse(const bool &boolean);
	public:
		Client(int const fd, Server *server);
		~Client();

		Client &operator=(Client const &other);
		Server *_server;
		void setResponse(const std::string &code = "200");
		void setPid(const int &pid);
		const int &getPid(void) const;
		void setCGIStatus(const int &status);
		const int &getCGIStatus(void) const;
		void	cleanRequest( void );
		void	setCGIFD(const int &fd);
		const int & getCGIFD(void);
		void	setParentToCGI(const int &fd);
		const int & getParentToCGI(void);
		void setUseBuffer(const bool &boolean);
		bool getUseBuffer(void) const;
		const int & getClientFd(void) const;
		bool const &isReadyToResponse(void) const; 
		void pushHeaderRequest(char *str, const int &n);
    	void setServerReq(Server *server);
		const t_state &whatToDo(void) const;
		const bool &getReadyToParseHeader(void) const;
		const bool &getReadyToParseBody(void) const;
		Request *getRequest(void);
		void afterResponse(void);
};

#endif
