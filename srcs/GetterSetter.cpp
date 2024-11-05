#include "../includes/utils.hpp"

int getSocketFd(int fd, int flag) {
	static int save;

	if (flag == SET) {
		save = fd;
	}
	return (save);
}

int getEpollFd(int fd, int flag) {
	static int save;

	if (flag == SET) {
		save = fd;
	}
	return (save);
}