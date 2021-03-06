/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 20:05:07 by ablanar           #+#    #+#             */
/*   Updated: 2021/04/28 14:01:48 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_CLASS_HPP
#define REQUEST_CLASS_HPP
#include <vector>
#include <string>
#include <iostream>
#include "Utility.hpp"
#include "./Header.class.hpp"

//501 if method is not recognized
//405 if method is recognized but not supported
class Request
{
	public:
		Request(void);
		Request(Request const &src);
		~Request(void);
		Request &operator=(Request const &src);
		// int read_request(int sd);
		int isMethod(std::string line);
		int startLineReader(std::string line);
		std::string getMethod(void); //
		void addHeader(std::string line);
		int getBytes(void);
		void setBody(std::string body);
		std::string getBody(void); //
		void setBytes(int bytes);
		std::string getState(void);
		void setState(std::string state);
		void print_headers(void);
		void setError(int error);
		int getError(void);
		std::string getPath(void) const;
		std::vector<Header> getHeaders(void); //
		void uri_handler(std::string str);
		int isHeaderPresent(std::string name, std::string value = "");
		void read_request(int sd);
		unsigned long contentLengthChecker(std::vector<Header> headers);
		std::vector<Header>::iterator getHeaderByName(std::string name);
		void ChunkedInterpretation(std::string chunk);
		long long getBodyLength() const;
		std::string getExtension(void);
	private:
		int _read_bytes;
		std::vector<Header> _headers;
		long long _content_length;
		std::string _status_line;
		std::string _method;
		std::string _body;
		std::string _query;
		std::string _path;
		std::string _fragment;
		int _error;

		std::string _state;
		std::string methods[8];
		std::string _buffer;
		int _chunked;
};
std::vector<std::string> split(std::string input, std::string word);
#endif
