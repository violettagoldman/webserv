/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 22:09:36 by ablanar           #+#    #+#             */
/*   Updated: 2021/04/01 16:21:08 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.class.hpp"
#include "../inc/Utility.hpp"
#include <sys/socket.h>
// #include "../inc/CGIHandler.class.hpp"

const std::string CRLF = "\r\n";
const std::string WSP = "\r ";
#define BUFFER_SIZE 1000000

int	ft_atoi_base(const char *str, const char *base);

std::vector<std::string> remove_spaces(std::vector<std::string> values)
{
	size_t pos;
	for (std::vector<std::string>::iterator it = values.begin(); it < values.end(); ++it)
	{
		if ((pos = WSP.find((*it)[0])) != std::string::npos)
			(*it).erase(0, 1);
		if ((pos = WSP.find((*it)[((*it).length()) - 1])) != std::string::npos)
			(*it).erase((*it).length() - 1, 1);
	}
	return values;
}

// Header *header_split(std::string str)
// {
// 	std::string header_name;
// 	size_t index = str.find_first_of(":");
// 	header_name = str.substr(0, index);
// 	if (header_name.find(" ") != std::string::npos)
// 	/*
// 	* A
// 	* server MUST reject any received request message that contains
// 	* whitespace between a header field-name and colon with a response code
// 	* of 400 (Bad Request).
// 	*/
// 		return NULL;
// 	std::string value = str.substr(index + 1);
// 	value.erase(value.find('\n'));
// 	std::vector<std::string> values = ft_split(value, ',');
// 	values = remove_spaces(values);
//
// 	Header *new_header = new Header(header_name, values);
// 	return new_header;
// }

std::vector<std::string> ft_split(std::string s, char c);

Request::Request(void) :
	_read_bytes(0)
{
	_error = 0;
   	methods[0] = "GET";
   	methods[1] = "HEAD";
   	methods[2] = "POST";
   	methods[3] = "PUT";
   	methods[4] = "DELETE";
   	methods[5] = "CONNECT";
   	methods[6] = "OPTIONS";
   	methods[7] = "TRACE";
}

Request::Request(Request const &src) :
	_read_bytes(src._read_bytes), _headers(src._headers),
	_content_length(src._content_length), _status_line(src._status_line), _method(src._method),
 _body(src._body), _query(src._query), _path(src._path), _fragment(src._fragment),
 _error(src._error), _state(src._state)
{}

Request::~Request(void)
{}

Request		&Request::operator=(Request const &src)
{
	_read_bytes = src._read_bytes;
	_headers = src._headers;
	_content_length = src._content_length;
	_status_line = src._status_line;
	_method = src._method;
	_body = src._body;
	_query = src._query;
	_path = src._path;
	_fragment = src._fragment;
	_error = src._error;
	_state = src._state;
	_buffer = src._buffer;

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
	for (int i = 0; i < 8; i++)
		if (check == methods[i])
			return i;
	return -1;
}

void Request::uri_handler(std::string str)
{
	std::string scheme;
	std::string authority;
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
}

int Request::startLineReader(std::string line)
{
	std::vector<std::string> elements;
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


void Request::ChunkedInterpretation(std::string chunk)
{
	int size;
	std::string info;

	size_t pos;

	while ((pos = chunk.find(CRLF)) != std::string::npos)
	{
		// std::cout << chunk.substr(0, pos) << std::endl;
		size = ft_atoi_base(chunk.substr(0, pos).c_str(), "0123456789abcdef");
		chunk.erase(0, pos + 2);
		// std::cout << "Size of chunk: "<< size << std::endl;
		// std::cout << "chunk again" << chunk << std::endl;
		pos = chunk.find(CRLF);
		info = chunk.substr(0, pos);
		// std::cout << "k" <<  info << "k"  << info.size()<< std::endl;
		if ((int)info.size() != size)
		{
			_error = 400;
			return ;
		}
		_body += info;
		// std::cout << _body << std::endl;
		chunk.erase(0, size + 2);
	}
}

void Request::read_request(int sd)
{
		char input[BUFFER_SIZE + 1];
		bzero(input, BUFFER_SIZE + 1);
		int bytes;
		size_t pos;
		size_t last;
		std::string body;
		std::string start_line;
		std::cout << "before reading" << std::endl;
		usleep(1000);
		bytes = recv(sd, input, BUFFER_SIZE, 0);
		if (bytes <= 0)
		{
			_state = "end";
			return ;
		}
		std::string to_interpret(input);
		// std::cout << "LOL SMOTRI: \n"   << "\"" << to_interpret << "\"" << std::endl;
		if (to_interpret.find("Transfer-Encoding: chunked") != std::string::npos || _state == "chunked")
		{
			_state = "chunked";
			_buffer += to_interpret;
		}
		// std::cout << "buffer: " <<"\"" << _buffer << "\"" << std::endl;
		if (_buffer.find("0\r\n\r\n") != std::string::npos)
		{
			_state = "read";
			to_interpret = _buffer;
			// std::cout << "Final request:\n" << to_interpret << std::endl;
		}
		// std::cout << "LOL SMOTRI: \n"   <<bytes << "\"" << to_interpret << "\"" << std::endl;
		if (bytes > 0 && _state != "chunked")
		{
			_state = "read";
			pos = to_interpret.find("\n");
			start_line = to_interpret.substr(0, pos - 1);
			startLineReader(start_line);
			last = to_interpret.find('\n', pos + 1);
			std::string one_header;
			// std::cout << to_interpret.size() << std::endl;
			while (pos + 1!= last && last != std::string::npos && to_interpret.substr(pos + 1, last - pos) != CRLF)
			{
				one_header = to_interpret.substr(pos + 1, last - pos);
				pos = last;
				// std::cout << one_header << isHeaderPresent("Transfer-Encoding", "chunked") << std::endl;
				last = to_interpret.find("\n", last + 1);
				// std::cout << "Pos: " << pos + 1 << std::endl;
				// std::cout << "Last: " << last << std::endl;
				addHeader(one_header);
			}
			if (!isHeaderPresent("Host"))
				setError(400);
			if (isHeaderPresent("Content-Length"))
			{
				unsigned long content_size = contentLengthChecker(getHeaders());
				if (content_size != 0)
				{
					body = to_interpret.substr(last + 1);
					if (content_size != body.length() || getError() == 400)
					{
						setState("error");
						setError(400);
					}
				setBody(body);
				}
			}
			if (isHeaderPresent("Transfer-Encoding", "chunked"))
			{

				// std::cout << "chunks: " << "\'" << to_interpret.substr(last + 1) << "\'" <<  std::endl;
				ChunkedInterpretation(to_interpret.substr(last + 1));
				// std::cout << "Body" << "\'" <<  _body <<  "\'" << std::endl;
			}

		}
		// else if (_state == "chunked" && isHeaderPresent("Transfer-Encoding", "chunked"))
		// {
		// 	pos = to_interpret.find(CRLF);
		// 	last = to_interpret.find('\n', pos + 1);
		// 	if (to_interpret.find('0') != std::string::npos && to_interpret.find(CRLF, pos + 1) != std::string::npos)
		// 		_state = "read";
		// 	else
		// 		_buffer += to_interpret;
		// 	std::cout << "Buffer: " << _buffer << std::endl;
		// 	// if (to_interpret.substr(pos + 1, last - pos) != CRLF || pos + 1!= last || last != std::string::npos)
		// 	// {
		// 		// _state = "read";
		// 	// }
		// 		// In chunked requests data length is in hex. transform hex to Int
		// 		// while ((bytes = recv(sd, input, BUFFER_SIZE, 0)) > 0)
		// 		// {
		// 		// 	to_interpret.assign(input, bytes);
		// 		// 	std::cout << to_interpret << std::endl;
		// 		// }
		//
		// }
		else if (bytes == -1)
			_state = "end";
}
// void Request::read_request(int sd)
// {
// 	char input[BUFFER_SIZE + 1];
// 	bzero(input, BUFFER_SIZE + 1);
// 	int bytes;
// 	size_t pos;
// 	size_t last;
// 	std::string body;
// 	std::string start_line;
// 	bytes = recv(sd, input, BUFFER_SIZE, 0);
// 	if (bytes == 0)
// 		_state = "end";
// 	std::string to_interpret(input);
// 	std::cout << "LOL SMOTRI: \n"  << bytes << to_interpret << std::endl;
// 	if (bytes > 0 && _state != "chunked")
// 	{
// 		_state = "read";
// 		pos = to_interpret.find("\n");
// 		start_line = to_interpret.substr(0, pos - 1);
// 		startLineReader(start_line);
// 		last = to_interpret.find('\n', pos + 1);
// 		std::string one_header;
// 		std::cout << to_interpret.size() << std::endl;
// 		while (pos + 1!= last && last != std::string::npos && to_interpret.substr(pos + 1, last - pos) != CRLF)
// 		{
//
// 			one_header = to_interpret.substr(pos + 1, last - pos);
// 			pos = last;
// 			std::cout << one_header << isHeaderPresent("Transfer-Encoding", "chunked") << std::endl;
// 			last = to_interpret.find("\n", last + 1);
// 			std::cout << "Pos: " << pos + 1 << std::endl;
// 			std::cout << "Last: " << last << std::endl;
// 			addHeader(one_header);
// 		}
// 		if (isHeaderPresent("Transfer-Encoding", "chunked"))
// 		{
// 			std::cout << "HEere " << one_header << std::endl;
// 			setState("chunked");
// 		}
// 		if (!isHeaderPresent("Host"))
// 			setError(400);
// 		if (isHeaderPresent("Content-Length"))
// 		{
// 			// std::cout << "Jerer" << std::endl;
// 			unsigned long content_size = contentLengthChecker(getHeaders());
// 			if (content_size != 0)
// 			{
// 				body = to_interpret.substr(last + 1);
// 				if (content_size != body.length() || getError() == 400)
// 				{
// 					setState("error");
// 					setError(400);
// 				}
// 			setBody(body);
// 			}
// 		}
// 	}
// 	else if (_state == "chunked" && isHeaderPresent("Transfer-Encoding", "chunked"))
// 	{
// 		pos = to_interpret.find(CRLF);
// 		last = to_interpret.find('\n', pos + 1);
// 		if (to_interpret.find('0') != std::string::npos && to_interpret.find(CRLF, pos + 1) != std::string::npos)
// 			_state = "read";
// 		else
// 			_buffer += to_interpret;
// 		std::cout << "Buffer: " << _buffer << std::endl;
// 		// if (to_interpret.substr(pos + 1, last - pos) != CRLF || pos + 1!= last || last != std::string::npos)
// 		// {
// 			// _state = "read";
// 		// }
// 			// In chunked requests data length is in hex. transform hex to Int
// 			// while ((bytes = recv(sd, input, BUFFER_SIZE, 0)) > 0)
// 			// {
// 			// 	to_interpret.assign(input, bytes);
// 			// 	std::cout << to_interpret << std::endl;
// 			// }
//
// 	}
// 	else if (bytes == -1)
// 		_state = "end";
// }

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
