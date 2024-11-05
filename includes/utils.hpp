#ifndef UTILS_HPP
#define UTILS_HPP

# define SET 0
# define GET 1

int getSocketFd(int fd, int flag);
int getEpollFd(int fd, int flag);


#endif