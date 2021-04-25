#include "Server.class.hpp"
// #define SO_SNDBUF 100000001
Server::Server(void)
{
}

Server::Server(Server const &src)
{
	*this = src;
}

Server::~Server(void)
{
}

Server					&Server::operator=(Server const &src)
{
	if (this != &src)
	{
		_host = src._host;
		_fd = src._fd;
		_port = src._port;
	}
	return (*this);
}

unsigned short	ft_htons(unsigned short x)
{
	x = x >> 8 | x << 8;
	return (x);
}

/*
* This method is used to setup the server by:
* 1. creting the socket
* 2. setting the address
* 3. binding the address to set the port
* @return int Error code
*/
int						Server::setup(VirtualHost conf)
{
	// int yes = 1;
	 int nSndBufferLen = 1000000001;
	 				     // 100000000
	 int nLen          = sizeof(int);
	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Couldn't initialize the socket" << std::endl;
		return (-1);
	}
	int n;
	unsigned int m = sizeof(n);
	getsockopt(_fd,SOL_SOCKET,SO_RCVBUF,(void *)&n, &m);
	std::cout <<"befor fcntl" <<  n  << std::endl;
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		return (-1);
	// getsockopt(_fd,SOL_SOCKET,,(void *)&n, &m);
	std::cout <<"fare fcntl" <<  n  << std::endl;
	setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, (char*)&nSndBufferLen, nLen);
	setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, (char*)&nSndBufferLen, nLen);
	std::cout <<"set fcntl" <<  n  << std::endl;
	_address.sin_family = AF_INET;
	if (conf.getListenHost().size())
		_address.sin_addr.s_addr = inet_addr(conf.getListenHost().c_str());
	else
		_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = ft_htons(conf.getListenIp());
	if (::bind(_fd, reinterpret_cast<struct sockaddr *>(&_address), sizeof(_address)) == -1)
	{
		std::cerr << "Couldn't bind the port " << conf.getListenIp() << std::endl;
		exit(1);
	}
	std::cout << "Listening on port " << conf.getListenIp() << std::endl;
	return (0);
}

/*
* This method is used to listen on a port
* and wait for a connection to be established
* @return int Error code
*/
int						Server::listen(void)
{
	if (::listen(_fd, -1) == -1)
	{
		std::cerr << "Couldn't listen to connections" << std::endl;
		return (-1);
	}
	return (0);
}

/*
* This method is used to accept connection from a client
* @return int Error code
*/
int						Server::accept(void)
{
	int	addrlen;
	int newSocket;
	int n;
	int yes = 1;
	unsigned int m = sizeof(n);
	addrlen = sizeof(_address);
	if ((newSocket = ::accept(_fd, (struct sockaddr *)&_address,
		(socklen_t *)&addrlen)) == -1)
	{
		std::cerr << "Couldn't accept the connection" << std::endl;
		return (-1);
	}
	getsockopt(newSocket, SOL_SOCKET,SO_RCVBUF,(void *)&n, &m);

	std::cout <<"befor fcntl" <<  n  << std::endl;
	if (fcntl(newSocket, F_SETFL, O_NONBLOCK) < 0)
		return (-1);
	setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	std::cout <<"after fcntl" <<  n  << std::endl;
	return (newSocket);
}

/*
* This method is used to write in the socket
* @return int Error code
*/
#define BUFFER_SIZE1 1000000
#include <string.h>
int					Server::send(int fd, std::string message, fd_set fds_write) const
{
	std::cout << message.size() << std::endl;
	// size_t len = message.size();
	// size_t offset = 0;
	// ssize_t result;
	// std::cout << len  << std::endl;
	// while (offset < len) {
	//   result = ::send(fd, message.substr(offset).c_str(), len - offset, 0);
	//   if (result < 0) {
	//     	std::cerr << "Could not send response" << std::endl;
	//   }
	//   offset += result;
	// }
	int kek;
	int len = message.size();
	int offset = 0;
	char buf[BUFFER_SIZE1 + 1];
	(void)fds_write;

	while (offset < len)
	{
		bzero(buf, BUFFER_SIZE1 + 1);
		memcpy(buf, message.c_str() + offset, BUFFER_SIZE1 > len - offset ? len - offset : BUFFER_SIZE1);
		// while (!FD_ISSET(fd , &fds_write) && offset < len)
		// {}
		kek = ::send(fd, buf, (BUFFER_SIZE1 > len - offset ? len - offset : BUFFER_SIZE1), 0);
		std::cout << "kek in loop" << kek << std::endl;
		if (kek < 0)
			std::cout << "error" << kek << std::endl;
		// 	break;
		offset += kek;
	}
	// if ((kek = write(fd, message.c_str(), message.size())) == (size_t)-1)
	// 	std::cerr << "Could not send response" << std::endl;
	std::cout << "kek:" << kek << std::endl;
	return (0);
}

/*
* This method is used to release all data
* @return int Error code
*/
int					Server::close(void)
{
	for (size_t i = 0; i < _clients.size(); i++)
		::close(_clients[i]);
	::close(_fd);
	return (0);
}

std::vector<int>	Server::getClients(void) const
{
	return (_clients);
}

std::string			Server::getHost(void) const
{
	return (_host);
}

std::string			Server::getPort(void) const
{
	return (_port);
}

int					Server::getFd(void) const
{
	return (_fd);
}

void				Server::addClient(int fd)
{
	_clients.push_back(fd);
}

void				Server::removeClient(int fd)
{
	_clients.erase(std::find(_clients.begin(), _clients.end(), fd));
}

void				Server::setHost(std::string host)
{
	this->_host = host;
}

void				Server::setPort(std::string port)
{
	this->_port = port;
}

void				Server::setFd(int fd)
{
	this->_fd = fd;
}
