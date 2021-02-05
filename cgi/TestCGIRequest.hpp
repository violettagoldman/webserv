/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestCGIRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:28:31 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/05 13:20:40 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTCGIREQUEST_HPP
# define TESTCGIREQUEST_HPP

# include "ICGIRequest.hpp"

class TestCGIRequest : public ICGIRequest {

public:

	TestCGIRequest();
	~TestCGIRequest();
	// TestCGIRequest(const TestCGIRequest &copy);
	// TestCGIRequest &operator= (const TestCGIRequest &operand);

private:
	std::string _remoteAddr;
	std::string _remoteHost;
	std::string _authType;
	std::string _remoteIdent;
	std::string _remoteUser;
	std::string _contentType;
	std::string _requestMethod;
	std::string _requestURI;
	std::string _serverPort;
	std::string _serverName;
	std::string _scriptFilename;
};

#endif