/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 22:09:36 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/14 20:32:50 by ablanar          ###   ########.fr       */
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
std::vector<std::string> split(std::string input, std::string word = "")
{
	std::vector<std::string> names;
	while (input.compare(word) != 0)
	{
		size_t index = input.find_first_of(" ");
		word = input.substr(0,index);
		input = input.substr(index+1, input.length());
		if (word.length() == 0)
			continue;
		names.push_back(word);
	}
	return names;
}

Request::Request(void)
{
	std::cout << "Defualt constructor for Request called\n";
}

Request::Request(Request const &src) :
	_headers(src._headers), _status_line(src._status_line), _method(_method)
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

int Request::read_request(int sd)
{
	char *line;
	int ret;

	ret = get_next_line(sd, &line);
	// std::cout << line;
	std::string toRead(line);
	std::cout << "From gnl" << toRead;
	return ret;
}
