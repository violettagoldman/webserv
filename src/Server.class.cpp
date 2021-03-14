#include "Server.class.hpp"

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
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Couldn't initialize the socket" << std::endl;
		return (-1);
	}
	_address.sin_family = AF_INET;
	if (conf.getListenHost().size())
		_address.sin_addr.s_addr = inet_addr(conf.getListenHost().c_str());
	else
		_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = ft_htons(conf.getListenIp());
	std::cout << "AAA: " << ft_htons(conf.getListenIp()) << std::endl;
	if (::bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) == -1)
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
	if (::listen(_fd, 10) == -1)
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

	addrlen = sizeof(_address);
	if ((newSocket = ::accept(_fd, (struct sockaddr *)&_address,
		(socklen_t *)&addrlen)) == -1)
	{
		std::cerr << "Couldn't accept the connection" << std::endl;
		return (-1);
	}
	return (newSocket);
}

/*
* This method is used to write in the socket
* @return int Error code
*/
int					Server::send(int fd, std::string message) const
{
	if (::send(fd, message.c_str(), message.size(), 0) == -1)
		std::cerr << "Could not send response" << std::endl;
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
