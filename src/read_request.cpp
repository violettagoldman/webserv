/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/24 17:34:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/02/10 00:14:29 by ablanar          ###   ########.fr       */
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
const std::string CRLF = "\r\n";

unsigned long contentLengthChecker(std::vector<Header *> headers, Request *req)
{
	std::vector<std::string> values;
	for (std::vector<Header *>::iterator it = headers.begin(); it < headers.end(); ++it)
		if ((*it)->getName() == "Content-Length")
			values = (*it)->getValue();
	if (values.size() > 1)
	{
		req->setError(400);
		return -1;
	}
	unsigned long size =  std::strtol(values[0].c_str(), NULL, 10);
	if (errno)
	{
		req->setError(400);
		return -1;
	}
	return size;

}

Header *header_split(std::string str)
{
	std::string header_name;
	size_t index = str.find_first_of(":");
	header_name = str.substr(0, index);
	std::string value = str.substr(index + 1);
	value.erase(value.find('\n'));
	std::vector<std::string> values = split(value, ", ");
	Header *new_header = new Header(header_name, values);
	if (!isValidHeader(header_name))
	{
		new_header->setError(-1);
		return new_header;
	}
	return new_header;
}

Request *read_request(int sd, Request *req)
{
	char input[BUFFER_SIZE];
	int bytes;
	int pos;
	int last;
	std::string body;
	std::string start_line;
	bytes = recv(sd, input, BUFFER_SIZE, 0);
	// bytes = read(fd, input, BUFFER_SIZE);
	if (bytes == 0)
		req->setState("chill");
	std::string to_interpret(input);
	std::cout << to_interpret;
	if (bytes > 0)
	{
		req->setState("read");
		Header *hed;
		pos = to_interpret.find("\n");
		start_line = to_interpret.substr(0, pos);
		req->startLineReader(start_line);
		last = to_interpret.find('\n', pos + 1);
		std::cout << to_interpret.length();
		std::string one_header;
		while (pos + 1 != last && to_interpret.substr(pos + 1, last - pos) != CRLF)
		{
			one_header = to_interpret.substr(pos + 1, last - pos);
			pos = last;
			last = to_interpret.find("\n", last + 1);
			hed = header_split(one_header);
			if (hed->isError())
				return req;
			req->addHeader(hed);
		}
		if (req->isHeaderPresent("Content-Length"))
		{
			unsigned long content_size = contentLengthChecker(req->getHeaders(), req);
			body = to_interpret.substr(last + 1);
			if (content_size != body.length())
			{
				req->setError(400);
				return req;
			}
			req->setBody(body);
		}
	}
	else if (bytes == -1)
		req->setState("end");
	return req;
}
