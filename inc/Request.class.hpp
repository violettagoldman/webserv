/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.class.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 20:05:07 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/17 18:39:29 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_CLASS_HPP
#define REQUEST_CLASS_HPP
#include <vector>
#include <string>
#include <iostream>


class Request
{
	public:
		Request(void);
		Request(Request const &src);
		~Request(void);
		Request &operator=(Request const &src);
		int read_request(int sd);
		int isMethod(std::string line);
		int startLineReader(std::string line);
		std::string getMethod(void);
	private:
		std::vector<std::string> _headers;
		std::string _status_line;
		std::string _method;
		int _error;

};

#endif
