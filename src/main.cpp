/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 21:34:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/13 16:56:28 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#define NONE 0
#define ABSOLUTEURI 1
#define ABSOLUTEPATH 2
#define AUTHORITY 3

/*
* source https://www.tutorialspoint.com/http/http_requests.htm
*/



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
*	This is the function to check the validity of the method
*	@param check String which contains method name to be checked to be splited.
*	@return int This returns position of the method in the methods array or -1
*	if method was not found in array
*/
int isMethod(std::string check)
{
	for (int i = 0; i < 9; i++)
		if (check == methods[i])
			return i;
	return -1;
}



void uri_handler(std::string str)
{
	std::string scheme;
	std::string authority;
	std::string path;
	std::string query;
	std::string fragment;
	std::size_t fragment_pos;
	std::size_t query_pos;

	if ((fragment_pos = str.find("#")) != std::string::npos)
	{
		fragment = str.substr(fragment_pos + 1);
		str.erase(fragment_pos);
	}
	if ((query_pos = str.find("?")) != std::string::npos)
	{
		query = str.substr(query_pos + 1);
		str.erase(query_pos);
	}
	std::cout << fragment << std::endl;
	std::cout << query;
}

int request(std::string line)
{
	std::vector<std::string> elements;

	if (line.find("  ") != std::string::npos)
		return 400;
	elements = split(line);
	if (elements.size() != 3 || elements[2] != "HTTP/1.1")
		return 404;
	if (isMethod(elements[0]) == -1)
		return 501;
	uri_handler(elements[1]);
	return 0;
}


int main(int argc, char **argv)
{
	std::vector<std::string> out;
	std::cout << request(argv[1]);
	// out = split(argv[1]);
	// for (std::vector<std::string>::iterator it = out.begin(); it != out.end(); ++it)
	// 	std::cout << *it << std::endl;
	return 0;
}
