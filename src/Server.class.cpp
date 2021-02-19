#include "../inc/Server.class.hpp"

Server::Server(void)
{
	std::cout << "Default constructor called\n";
}

Server::Server(Server const &src)
{
	std::cout << "Copy constructor called\n";
	*this = src;
}

Server::~Server(void)
{
	std::cout << "Destructor called\n";
}

Server					&Server::operator=(Server const &src)
{
	std::cout << "Assignation operator called\n";
	if (this != &src)
	{
		_host = src._host;
		_fd = src._fd;
		_port = src._port;
	}
	return (*this);
}

/*
* This method is used to setup the server by:
* 1. creting the socket
* 2. setting the address
* 3. binding the address to set the port
* @return int Error code
*/
int						Server::setup(void)
{
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Couldn't initialize the socket" << std::endl;
		return (-1);
	}
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY; //change by ft custom function
	_address.sin_port = htons(8880); //same
	if (::bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) == -1)
	{
		std::cerr << "Couldn't bind the port" << std::endl;
		return (-1);
	}
	return (0);
}

/*
* This method is used to run the server
* @return int Error code
*/
int						Server::run(void)
{
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
* This method is used to read from the socket
* @return int Error code
*/	
std::string				Server::recieve(void)
{
	char		buffer[4096];
	std::string	request;
	int			res;

	// while (res)
	// {
		// ft_memset(buffer, 0, 4096);
		if ((res = recv(_fd, buffer, 4095, 0)) == -1)
			std::cerr << "Coudn't recieve the request" << std::endl;
		request += std::string(buffer);

	// }
	return (request);
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
* This method is used to end read/write in a socket
* @return int Error code
*/
int					Server::shutdown(void)
{
	return (0);
}

/*
* This method is used to release all data
* @return int Error code
*/
int					Server::close(void)
{
	// if (_socket > 0)
	// 	::close(_fd);
	// _socket = -1;
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
