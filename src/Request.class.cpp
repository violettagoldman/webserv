/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 22:09:36 by ablanar           #+#    #+#             */
/*   Updated: 2021/02/19 13:35:51 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.class.hpp"
#include "../inc/Utility.hpp"
#include <sys/socket.h>
const std::string CRLF = "\r\n";
const std::string WSP = "\r ";
#define BUFFER_SIZE 1000000


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



/*
*	This is the function to split string.
*	@param input String to be splited.
*	@param word String by which input is going to be splited.
*	@return vector<string> This returns vector of strings that are in input
*	and separated by word.
*/
// std::vector<std::string> split(std::string input, std::string key = "")
// {
// 	std::vector<std::string> names;
// 	std::string word;
// 	while (input.compare(word) != 0)
// 	{
// 		size_t index = input.find_first_of(key);
// 		word = input.substr(0,index);
// 		input = input.substr(index+1, input.length());
// 		if (word.length() == 0)
// 			continue;
// 		names.push_back(word);
// 	}
// 	return names;
// }

std::vector<std::string> ft_split(std::string s, char c);

Request::Request(void) :
	_read_bytes(0)
{

   	methods[0] = "GET";
   	methods[1] = "HEAD";
   	methods[2] = "POST";
   	methods[3] = "PUT";
   	methods[4] = "DELETE";
   	methods[5] = "CONNECT";
   	methods[6] = "OPTIONS";
   	methods[7] = "TRACE";
	std::cout << "Defualt constructor for Request called\n";
}

Request::Request(Request const &src) :
	_read_bytes(src._read_bytes), _headers(src._headers),
	_content_length(src._content_length), _status_line(src._status_line), _method(src._method),
 _body(src._body), _query(src._query), _path(src._path), _fragment(src._fragment),
 _error(src._error), _state(src._state)
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
	std::cout << line << std::endl;
	if (line.find("  ") != std::string::npos)
		return 400;
	elements = ft_split(line, ' ');
	if (elements.size() != 3 || elements[2] != "HTTP/1.1")
		return 401;
	if (isMethod(elements[0]) == -1)
		return 501;
	_method = elements[0];
	uri_handler(elements[1]);
	return 0;
}

void Request::addHeader(std::string line)
{
	Header header(line);
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
	for (std::vector<Header>::iterator it = _headers.begin(); it < _headers.end(); ++it)
		(*it).print_out();

}

void Request::setBody(std::string body)
{
	_body = body;
}

std::string Request::getBody(void)
{
	return _body;
}

void Request::setError(int error)
{
	_error = error;
}

int Request::getError(void)
{
	return _error;
}

std::string Request::getPath(void) const
{
	return _path;
}
int Request::isHeaderPresent(std::string name, std::string value)
{
	for (std::vector<Header>::iterator it = _headers.begin(); it < _headers.end(); ++it)
		if (name == (*it).getName())
		{
			if (value != "")
				return ((*it).checkValue(value));
			return 1;
		}
	return 0;
}
unsigned long Request::contentLengthChecker(std::vector<Header> headers)
{
	std::vector<std::string> values;
	for (std::vector<Header>::iterator it = headers.begin(); it < headers.end(); ++it)
		if ((*it).getName() == "Content-Length")
			values = (*it).getValue();
	if (values.size() > 1)
	{
		setError(400);
		return 0;
	}
	unsigned long size =  std::strtol(values[0].c_str(), NULL, 10);
	if (errno)
	{
		setError(400);
		return 0;
	}
	return size;

}

void Request::read_request(int sd)
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
		_state = "chill";
	std::string to_interpret(input);
	if (bytes > 0)
	{
		_state = "read";
		pos = to_interpret.find("\n");
		start_line = to_interpret.substr(0, pos - 1);
		startLineReader(start_line);
		last = to_interpret.find('\n', pos + 1);
		std::string one_header;
		while (pos + 1 != last && to_interpret.substr(pos + 1, last - pos) != CRLF)
		{
			one_header = to_interpret.substr(pos + 1, last - pos);
			pos = last;
			last = to_interpret.find("\n", last + 1);
			// if (hed.isError())
			// {
			// 	setState("error");
			// 	setError(400);
			// }
			addHeader(one_header);
		}
		if (isHeaderPresent("Content-Length"))
		{
			unsigned long content_size = contentLengthChecker(getHeaders());
			body = to_interpret.substr(last + 1);
			if (content_size != body.length() || getError() == 400)
			{
				setState("error");
				setError(400);
			}
			setBody(body);
		}
		if (isHeaderPresent("Transfer-Encoding", "chunked"))
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
		_state = "end";
}

std::vector<Header>::iterator Request::getHeaderByName(std::string name)
{
	for (std::vector<Header>::iterator it = _headers.begin(); it < _headers.end(); ++it)
		if ((*it).getName() == name)
			return it;
	return _headers.end();
}
std::vector<Header> Request::getHeaders(void)
{
	return _headers;
}
