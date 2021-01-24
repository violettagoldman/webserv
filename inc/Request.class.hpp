/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 20:05:07 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/24 21:28:44 by ablanar          ###   ########.fr       */
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
		std::string getMethod(void);
		void addHeader(Header *header);
		int getBytes(void);
		void setBytes(int bytes);
		std::string getState(void);
		void setState(std::string state);
	private:
		int _read_bytes;
		std::vector<Header *> _headers;
		std::string _status_line;
		std::string _method;
		int _error;
		int _content_length;
		std::string _state;
};
std::vector<std::string> split(std::string input, std::string word);
#endif
