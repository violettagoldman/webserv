/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 13:14:04 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/13 16:56:31 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
// #include <sstream>
#include <string>
#include <vector>
#include <map>

#include <string>
#include <string_view>

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



int check_validity(std::string header)
{
	for (int i = 0; i < 18; ++i)
	{
		if (header == accepted_headers[i])
			return 1;
	}
	return 0;
}

int header_split(std::string str, std::map<std::string, std::vector<std::string> >& headers)
{
	std::string header_name;
	size_t index = str.find_first_of(":");
	header_name = str.substr(0, index);
	if (!check_validity(header_name))
		return -1;
	std::string value = str.substr(index + 1, str.length());
	std::vector<std::string> values = split(value, ", ");
	// for (std::vector<std::string>::iterator it = values.begin(); it < values.end(); ++it)
	// 	std::cout << "value: " << *it << '\n';
	headers.insert(std::pair<std::string, std::vector<std::string> >(header_name, values));
	return 1;
}
void print_map(std::map<std::string, std::vector<std::string> > & m)
{
    for (std::map<std::string, std::vector<std::string> >::iterator it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << " = " << it->second[0] << "; " << std::endl;
    }
    std::cout << "\n";
}

int main()
{
	int fd;
	std::map<std::string, std::vector<std::string> > headers;
	std::string line;
	std::ifstream infile("example.txt");

	while (std::getline(infile, line))
	{
		// std::cout << line;
		if (header_split(line, headers) == -1)
			std::cout << "Error in header name\n";
		// std::cout << check_validity(line) << '\n';
	}
	print_map(headers);
	return (0);
}
