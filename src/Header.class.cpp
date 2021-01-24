/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 20:04:49 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/22 01:07:50 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Header.class.hpp"

std::string accepted_headers[18] = {
	"Accept-Charsets",
	"Accept-Language",
	"Allow",
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
	"WWW-Authenticate"
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
	for (int i = 0; i < 18; ++i)
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
