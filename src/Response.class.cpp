#include "Response.class.hpp"

Response::Response(void)
{
	std::cout << "Default constructor called\n";
}

Response::Response(Response const &src)
{
	std::cout << "Copy constructor called\n";
	*this = src;
}

Response::~Response(void)
{
	std::cout << "Destructor called\n";
}

Response		&Response::operator=(Response const &src)
{
	std::cout << "Assignation operator called\n";
	if (this != &src)
	{
	}
	return (*this);
}

std::string 	Response::serialize() const
{
	return ("");
}