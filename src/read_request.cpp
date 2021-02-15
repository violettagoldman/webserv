/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/24 17:34:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/02/15 20:58:31 by ablanar          ###   ########.fr       */
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
const std::string WSP = "\r ";
// std::vector<std::string> ft_split(std::string input, char key);


/*
* A Host header field must be sent in all HTTP/1.1 request messages.
* A 400 (Bad Request) status code may be sent to any HTTP/1.1 request message
* that lacks a Host header field or that contains more than one.
*/

std::vector<std::string> ft_split(std::string s, char c)
{
	std::vector<std::string> ret;
	int i = 0;
	size_t pos;
	while ((pos = s.find(c, i)) != std::string::npos)
	{
		ret.push_back(s.substr(i, pos-i));
		i = pos+1;
	}
	ret.push_back(s.substr(i));
	return ret;
}
unsigned long contentLengthChecker(std::vector<Header *> headers, Request *req)
{
	std::vector<std::string> values;
	for (std::vector<Header *>::iterator it = headers.begin(); it < headers.end(); ++it)
		if ((*it)->getName() == "Content-Length")
			values = (*it)->getValue();
	if (values.size() > 1)
	{
		req->setError(400);
		return 0;
	}
	unsigned long size =  std::strtol(values[0].c_str(), NULL, 10);
	if (errno)
	{
		req->setError(400);
		return 0;
	}
	return size;

}

std::vector<std::string> remove_spaces(std::vector<std::string> values)
{
	size_t pos;
	for (std::vector<std::string>::iterator it = values.begin(); it < values.end(); ++it)
	{
		if ((pos = WSP.find((*it)[0])) != std::string::npos)
			(*it).erase(0, 1);
		if ((pos = WSP.find((*it)[((*it).length())])) != std::string::npos)
			(*it).erase((*it).length() - 1, 1);
	}
	return values;
}
Header *header_split(std::string str)
{
	std::string header_name;
	size_t index = str.find_first_of(":");
	header_name = str.substr(0, index);
	if (header_name.find(" ") != std::string::npos)
	/*
	* A
	* server MUST reject any received request message that contains
	* whitespace between a header field-name and colon with a response code
	* of 400 (Bad Request).
	*/
		return NULL;
	// std::cout << header_name << std::endl;
	std::string value = str.substr(index + 1);
	value.erase(value.find('\n'));
	std::vector<std::string> values = ft_split(value, ',');
	values = remove_spaces(values);
	// std::cout << values[0] << std::endl;
	Header *new_header = new Header(header_name, values);
	// new_header->print_out();
	// if (!isValidHeader(header_name))
	// {
	// 	new_header->setError(-1);
	// 	return new_header;
	// }
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
	if (bytes > 0)
	{
		req->setState("read");
		Header *hed;
		pos = to_interpret.find("\n");
		start_line = to_interpret.substr(0, pos - 1);
		int kek = req->startLineReader(start_line);
		std::cout << kek << std::endl;
		last = to_interpret.find('\n', pos + 1);
		std::string one_header;
		while (pos + 1 != last && to_interpret.substr(pos + 1, last - pos) != CRLF)
		{
			one_header = to_interpret.substr(pos + 1, last - pos);
			pos = last;
			last = to_interpret.find("\n", last + 1);
			hed = header_split(one_header);
			if (hed->isError())
			{
				req->setState("error");
				req->setError(400);
				return req;
			}
			req->addHeader(hed);
		}
		if (req->isHeaderPresent("Content-Length"))
		{
			unsigned long content_size = contentLengthChecker(req->getHeaders(), req);
			body = to_interpret.substr(last + 1);
			if (content_size != body.length() || req->getError() == 400)
			{
				req->setState("error");
				req->setError(400);
				return req;
			}
			req->setBody(body);
		}
		if (req->isHeaderPresent("Transfer-Encoding", "chunked"))
		{
			//In chunked requests data length is in hex. transform hex to Int
			while ((bytes = recv(sd, input, BUFFER_SIZE, 0)) > 0)
			{
				to_interpret.assign(input, bytes);
				std::cout << to_interpret << std::endl;
			}
		}
	}
	else if (bytes == -1)
		req->setState("end");
	return req;
}
