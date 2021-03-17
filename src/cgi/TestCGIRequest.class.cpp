/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestCGIRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:29:08 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/06 15:54:35 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TestCGIRequest.class.hpp"

TestCGIRequest::TestCGIRequest(
	std::string method,
	std::string body,
	std::vector<Header> headers,
	// std::string query,
	std::string path) :
	_method(method), _body(body), _headers(headers),
	_path(path)
{
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

// std::string TestCGIRequest::getQuery(void)
// {
// 	return _query;
// }

std::string TestCGIRequest::getPath(void)
{
	return _path;
}

// std::string TestCGIRequest::getFragment(void)
// {
// 	return _fragment;
// }
