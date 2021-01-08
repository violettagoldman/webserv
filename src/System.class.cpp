#include "System.class.hpp"

System::System(void)
{
	std::cout << "Default constructor called\n";
}

System::System(System const &src)
{
	std::cout << "Copy constructor called\n";
	*this = src;
}

System::~System(void)
{
	std::cout << "Destructor called\n";
}

System		&System::operator=(System const &src)
{
	std::cout << "Assignation operator called\n";
	if (this != &src)
	{
		// _host = src._host;
		// _fd = src._fd;
		// _port = src._port;
	}
	return (*this);
}

void    System::clearSocketSet(void)
{
    FD_ZERO(_fd_set);
}

void    System::addMasterSocket(int masterSocket)
{
    FD_SET(masterSocket, _fd_set); 
}

fd_set  *System::getFdSet(void) const
{
    return (this->_fd_set);
}

void    System::setFdSet(fd_set *fd_set)
{
    this->_fd_set = fd_set;
}