/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestCGIRequest.class.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:28:31 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/25 17:50:54 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTCGIREQUEST_HPP
# define TESTCGIREQUEST_HPP

// # include "ICGIRequest.class.hpp"
#include "Request.class.hpp"

// A class that mocks the behavior of Request to test how its data
// is processed by CGIHandler

class TestCGIRequest : public Request {

public:

	TestCGIRequest();
	TestCGIRequest(const TestCGIRequest &copy);
	TestCGIRequest &operator=(const TestCGIRequest &operand);
	~TestCGIRequest();

	TestCGIRequest(std::string method,
	std::string body,
	std::vector<Header> headers,
	std::string path);
	// ~TestCGIRequest();

	std::string getMethod(void);
	std::string getBody(void);
	std::vector<Header> getHeaders(void);
	// std::string getQuery(void);
	std::string getPath(void);
	// std::string getFragment(void);
	// TestCGIRequest(const TestCGIRequest &copy);
	// TestCGIRequest &operator= (const TestCGIRequest &operand);

private:

	std::string _method;
	std::string _body;
	std::vector<Header> _headers;
	std::string _path;




};

#endif