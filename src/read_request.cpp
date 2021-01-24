/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/24 17:34:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/24 18:45:28 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define BUFFER_SIZE 1000000
//just really big number
#include <unistd.h>
#include <string>
#include <iostream>
#include <fcntl.h>
#include "../inc/Request.class.hpp"
#include "../inc/Header.class.hpp"
#include <sys/socket.h>


Header *header_split(std::string str)
{
	std::string header_name;
	size_t index = str.find_first_of(":");
	header_name = str.substr(0, index);
	std::string value = str.substr(index + 1, str.length());
	std::vector<std::string> values = split(value, ", ");
	Header *new_header = new Header(header_name, values);
	if (!isValidHeader(header_name))
	{
		new_header->setError(-1);
		return new_header;
	}
	// for (std::vector<std::string>::iterator it = values.begin(); it < values.end(); ++it)
	// 	std::cout << "value: " << *it << '\n';
	// headers.insert(std::pair<std::string, std::vector<std::string> >(header_name, values));
	return new_header;
}

Request *read_request(int sd, Request *req)
{
	char input[BUFFER_SIZE];
	int bytes;
	int pos;
	int last;
	// Request *req;
	std::string start_line;
	int fd = open("./test.txt", O_RDONLY);
	bytes = recv(sd, input, BUFFER_SIZE, 0);
	if (bytes == 0)
	{
		req->setState("chill");
		return req;
	}
	std::string buf(input);
	std::string to_interpret(buf);
	std::string headers;
	std::cout << "In reader: " << bytes << to_interpret << "\n";
	// probably need to do while for chunk
	if (bytes > 0)
	{
		req->setState("read");
		// req->setBytes(bytes);
		// req = new Request();
		Header *hed;
		pos = to_interpret.find("\n");
		start_line = to_interpret.substr(0, pos);
		req->startLineReader(start_line);
		last = to_interpret.find('\n', pos + 1);
		std::string one_header;
		while (pos + 1 != last)
		{
			one_header = to_interpret.substr(pos + 1, last - pos);
			pos = last;
			last = to_interpret.find("\n", last + 1);
			std::cout << "Header " << one_header;
			hed = header_split(one_header);
			if (hed->isError())
				return NULL;
			req->addHeader(hed);
		}
	}
	else if (bytes == -1)
	{
		// delete req;
		req->setState("end");
		return NULL;
	}
	else
		return req;
	// 4 step reader
	// start_line
	// headers
	// if content length - body
	//chunk read
	return req;
}
