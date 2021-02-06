/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 20:04:49 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/30 13:56:32 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Header.class.hpp"

std::string accepted_headers[23] = {
	"Accept-Charsets",
	"Accept-Language",
	"Allow",
	"Postman-Token", //special for postman
	"Authorization",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-Type",
	"Date",
	"Host",
	"Last-Modified",
	"Location",
	"Referer",
	"Retry-After",
	"Server",
	"Transfer-Encoding",
	"User-Agent",
	"WWW-Authenticate",
	"Accept",
	"Cache-Control",
	"Connection",
	"Accept-Encoding"
};

Header::Header(void)
{}

Header::Header(std::string name, std::vector<std::string> value):
	_name(name), _values(value), _error(0)
{}

Header::~Header(void)
{}

Header::Header(Header const &src):
	_name(src._name), _values(src._values), _error(src._error)
{}

int isValidHeader(std::string name)
{
	for (int i = 0; i < 23; ++i)
	{
		if (name == accepted_headers[i])
			return 1;
	}
	return 0;
}


Header &Header::operator=(Header const &src)
{
	_values = src._values;
	_name = src._name;
	_error = src._error;
	return *this;
}

int Header::isError(void)
{
	if (_error == -1)
		return 1;
	return 0;
}
void Header::setError(int i)
{
	_error = i;
}

std::string Header::getName(void)
{
	return _name;
}

int Header::checkValue(std::string value)
{
	for (std::vector<std::string>::iterator it = _values.begin(); it < _values.end(); ++it)
		if (value == (*it))
			return 1;
	return 0;
}

void Header::print_out(void)
{
	std::cout << _name << " : ";
	for (std::vector<std::string>::iterator it = _values.begin(); it < _values.end(); ++it)
		std::cout << *it << ", ";
	// std::cout << "\n";
}
