#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../RawBits/RawBits.hpp"
#include "../Request/Request.hpp"

class Server;

class Client {
	private:
		const int		_fd;
		int				_pipeFD;
		int				_ParentToCGI;
		int				_pid;
		int				_CGIStatus;
		bool			_useBuffer;
		Request			*_request;
		Server			*_server;

	public:
		Client(int const &fd, Server *server);
		~Client();

		/*		SETTER		*/
		void			setResponse(const std::string &code = "200");
		void			setPid(const int &pid);
		void			setCGIStatus(const int &status);
		void			setCGIFD(const int &fd);
		void			setUseBuffer(const bool &boolean);
		Request			*getRequest(void);

		/*		GETTER		*/
		const int		&getPid(void) const;
		const int		&getCGIStatus(void) const;
		const int		&getCGIFD(void);
		void			setParentToCGI(const int &fd);
		const int		&getParentToCGI(void);
		bool			getUseBuffer(void) const;
		const int		&getClientFd(void) const;
		const t_state	&whatToDo(void) const;

		void			cleanRequest(void);
		void			pushHeaderRequest(const char *str, const int &n);
		void			afterResponse(void);
};

#endif
