#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client {
	private:
		char *str;
		int _fd;
	public:
		Client();
		~Client();

		int getClientFd(void) const;
		void setClientFd(int fd);
};

#endif