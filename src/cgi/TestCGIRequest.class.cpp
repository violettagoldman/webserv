/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestCGIRequest.class.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:29:08 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/25 17:52:19 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TestCGIRequest.class.hpp"

TestCGIRequest::TestCGIRequest()
{
}

TestCGIRequest::TestCGIRequest(const TestCGIRequest &copy) :
	Request(copy),
	_method(copy._method),
	_body(copy._body),
	_headers(copy._headers),
	_path(copy._path)
{
	
}

TestCGIRequest &TestCGIRequest::operator=(const TestCGIRequest &operand)
{
	_method = operand._method;
	_body = operand._body;
	_headers = operand._headers;
	_path = operand._path;
	return *this;
}

TestCGIRequest::~TestCGIRequest()
{
}

TestCGIRequest::TestCGIRequest(
	std::string method,
	std::string body,
	std::vector<Header> headers,
	// std::string query,
	std::string path) :
	_method(method), _body(body), _headers(headers),
	_path(path)
{
	std::string longValue;
	std::vector<std::string> valueVector;
	std::string finalString;

	for (size_t i = 0; i < headers.size(); ++i)
	{
		valueVector = headers[i].getValue();
		longValue = "";
		for (size_t j = 0; j < valueVector.size(); j++)
			longValue += valueVector[j];

		Request::addHeader(headers[i].getName() + ": " + longValue + "\n");
	}
	// std::cout << "Constructor got: " << headers.size() << std::endl;
}

std::string TestCGIRequest::getMethod(void)
{
	return _method;
}

std::string TestCGIRequest::getBody(void)
{
	return _body;
}

std::vector<Header> TestCGIRequest::getHeaders(void)
{
	return _headers;
}

std::string TestCGIRequest::getPath(void)
{
	return _path;
}
