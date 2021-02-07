/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 22:09:36 by ablanar           #+#    #+#             */
/*   Updated: 2021/02/07 15:21:28 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.class.hpp"
#include "../inc/get_next_line.h"
std::string methods[8] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"CONNECT",
	"OPTIONS",
	"TRACE"
};
/*
*	This is the function to split string.
*	@param input String to be splited.
*	@param word String by which input is going to be splited.
*	@return vector<string> This returns vector of strings that are in input
*	and separated by word.
*/
std::vector<std::string> split(std::string input, std::string key = "")
{
	std::vector<std::string> names;
	std::string word;
	while (input.compare(word) != 0)
	{
		size_t index = input.find_first_of(key);
		word = input.substr(0,index);
		input = input.substr(index+1, input.length());
		if (word.length() == 0)
			continue;
		names.push_back(word);
	}
	return names;
}

Request::Request(void) :
	_read_bytes(0)
{
	std::cout << "Defualt constructor for Request called\n";
}

Request::Request(Request const &src) :
	_headers(src._headers), _status_line(src._status_line), _method(_method),
	_content_length(src._content_length), _body(src._body)
{
	std::cout << "Copy constructor for Request called\n";
}

Request::~Request(void)
{
	std::cout << "Default destructor for Request called\n";
}

Request		&Request::operator=(Request const &src)
{
	std::cout << "Assignation operator called\n";
	_headers = src._headers;
	_status_line = src._status_line;
	_method = src._method;
	_content_length = src._content_length;
	_body = src._body;
	return *this;
}

/*
*	This is the function to check the validity of the method
*	@param check String which contains method name to be checked to be splited.
*	@return int This returns position of the method in the methods array or -1
*	if method was not found in array
*/
int Request::isMethod(std::string check)
{
	for (int i = 0; i < 9; i++)
		if (check == methods[i])
			return i;
	return -1;
}

void Request::uri_handler(std::string str)
{
	std::string scheme;
	std::string authority;
	std::string path;
	// std::string query;
	// std::string fragment;
	std::size_t fragment_pos;
	std::size_t query_pos;

	if ((fragment_pos = str.find("#")) != std::string::npos)
	{
		_fragment = str.substr(fragment_pos + 1);
		str.erase(fragment_pos);
	}
	if ((query_pos = str.find("?")) != std::string::npos)
	{
		_query = str.substr(query_pos + 1);
		str.erase(query_pos);
	}
	_path = str;
	// std::cout << fragment << std::endl;
	// std::cout << query;
}

int Request::startLineReader(std::string line)
{
	std::vector<std::string> elements;

	if (line.find("  ") != std::string::npos)
		return 400;
	elements = split(line);
	if (elements.size() != 3 || elements[2] != "HTTP/1.1")
		return 404;
	if (isMethod(elements[0]) == -1)
		return 501;
	_method = elements[0];
	uri_handler(elements[1]);
	return 0;
}

void Request::addHeader(Header *header)
{
	_headers.push_back(header);
}

std::string Request::getMethod(void)
{
	return _method;
}

int Request::getBytes(void)
{
	return _read_bytes;
}

void Request::setBytes(int bytes)
{
	_read_bytes = bytes;
}

std::string Request::getState(void)
{
	return _state;
}

void Request::setState(std::string state)
{
	_state = state;
}

void Request::print_headers(void)
{
	for (std::vector<Header *>::iterator it = _headers.begin(); it < _headers.end(); ++it)
		(*it)->print_out();

}

void Request::setBody(std::string body)
{
	_body = body;
}

std::string Request::getBody(void)
{
	return _body;
}

int Request::isHeaderPresent(std::string name, std::string value)
{
	for (std::vector<Header *>::iterator it = _headers.begin(); it < _headers.end(); ++it)
		if (name == (*it)->getName())
		{
			if (value != "")
				return ((*it)->checkValue(value));
			return 1;
		}
	return 0;
}

std::vector<Header *> Request::getHeaders(void)
{
	return _headers;
}
