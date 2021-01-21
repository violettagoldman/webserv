#include "Response.class.hpp"

Response::Response(void)
{
	//test values
	_statusCode = 200;
	_statusCodeTranslation[200] = "OK";
	_headers["Date"] = "Sat, morning";
	_headers["Server"] = "Webserv/1.0 (Unix)";
	_body = "My body";
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

std::string 	Response::serialize()
{
	std::string res;

	_headers["Content-Length"] = ft_itoa(_body.size());
	res = "HTTP/1.1 " + ft_itoa(_statusCode) + " " + _statusCodeTranslation[_statusCode] + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		res += it->first + ": " + it->second + "\r\n";
	}
	res += "\r\n";
	res += _body;
	return (res);
}