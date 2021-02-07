/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 20:05:07 by ablanar           #+#    #+#             */
/*   Updated: 2021/02/06 15:45:07 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_CLASS_HPP
#define REQUEST_CLASS_HPP
#include <vector>
#include <string>
#include <iostream>
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
		void addHeader(Header *header);
		int getBytes(void);
		void setBody(std::string body);
		std::string getBody(void); //
		void setBytes(int bytes);
		std::string getState(void);
		void setState(std::string state);
		void print_headers(void);
		std::vector<Header *> getHeaders(void); //
		void uri_handler(std::string str);
		int isHeaderPresent(std::string name, std::string value = "");
	private:
		int _read_bytes;
		std::vector<Header *> _headers;
		std::string _status_line;
		std::string _method;
		std::string _body;
		std::string _query;
		std::string _path;
		std::string _fragment;
		int _error;
		int _content_length;
		std::string _state;
};
std::vector<std::string> split(std::string input, std::string word);
#endif
