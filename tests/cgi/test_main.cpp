/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 12:15:59 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/07 20:32:28 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.class.hpp"
#include "TestCGIRequest.class.hpp"
#include "femtotest.hpp"
#include <cstdlib>
#include <map>

std::vector<std::string> ft_split(std::string s, char c)
{
	std::vector<std::string> ret;
	int i = 0;
	size_t pos;
	while ((pos = s.find(c, i)) != std::string::npos)
	{
		ret.push_back(s.substr(i, pos-i));
		i = pos+1;
	}
	ret.push_back(s.substr(i));
	return ret;
}

int main(void)
{

	const char *pwd = std::getenv("PWD");

	if (pwd == NULL)
	{
		std::cout << "Please set $PWD, we need it for tests" << std::endl;
		exit(-1);
	}
	std::string s_pwd(pwd);

	out("CGI Variables test");

	CGIRequest cr2 = {
		"127.0.0.1", // remoteAddr
		"testRemoteHost", // remoteHost
		"testAuthType", // authType
		"testRemoteIdent", // remoteIdent
		"testRemoteUser", // remoteUser
		"testContentType", // contentType
		"GET", // requestMethod
		"http://example.com/cgi/test.php", // requestURI
		"80", // serverPort
		"example.com", // serverName
		s_pwd + "/cgi/test-cgi.php", // scriptFilename
		"/usr/local/bin/php-cgi"
	};

	CGIHandler h2("", cr2);

	std::string resp2 = h2.getCgiResponse();

	std::vector<std::string> lines = ft_split(resp2, '\n');

	std::map<std::string, std::string> envVarMap;

	int j = 0;
	while ((int)lines[j].at(0) != 13)
		j++;


	std::string key;
	std::string value;
	std::vector<std::string> l;
	while (j != lines.size())
	{
		l = ft_split(lines[j], ' ');
		if (l.size() == 2)
		{
			key = l[0];
			value = l[1];
			key.erase(key.size()-1, 1);
			envVarMap[key] = value;
		}
		j++;
	}

	check(envVarMap["REMOTE_ADDR"] == "127.0.0.1");
	check(envVarMap["REMOTE_HOST"] == "testRemoteHost");
	check(envVarMap["REMOTE_IDENT"] == "testRemoteIdent");
	check(envVarMap["REMOTE_USER"] == "testRemoteUser");
	check(envVarMap["AUTH_TYPE"] == "testAuthType");
	check(envVarMap["CONTENT_TYPE"] == "testContentType");
	check(envVarMap["REQUEST_METHOD"] == "GET");
	check(envVarMap["REQUEST_URI"] == "http://example.com/cgi/test.php");
	check(envVarMap["SERVER_PORT"] == "80");
	check(envVarMap["SERVER_NAME"] == "example.com");
	check(envVarMap["SCRIPT_FILENAME"] != "");
	check(envVarMap["SCRIPT_FILENAME"].find("test-cgi.php") != std::string::npos);
	check(envVarMap["SCRIPT_NAME"].find("test-cgi.php") != std::string::npos);
	check(envVarMap["PATH_INFO"] == "http://example.com/cgi/test.php");
	check(envVarMap["GATEWAY_INTERFACE"] == "CGI/1.1");
	check(envVarMap["SERVER_PROTOCOL"] == "HTTP/1.1");
	check(envVarMap["SERVER_SOFTWARE"] == "Webserv/1.1");

	out("TestCGIRequest | Constructor");

	std::vector<Header *> hds;

	std::vector<std::string> values;
	values.push_back("testValue1");
	values.push_back("testValue2");

	Header *header = new Header("testKey", values);

	hds.push_back(header);

	TestCGIRequest tcr("GET",
		"testBody",
		hds,
		"testQuery",
		"testPath",
		"testFragment"
	);

	check(tcr.getMethod() == "GET");
	check(tcr.getBody() == "testBody");

	check(tcr.getHeaders().size() == 1);
	check(tcr.getHeaders()[0]->getName() == "testKey");
	check(tcr.getHeaders()[0]->getValue().size() == 2);
	check(tcr.getHeaders()[0]->getValue()[0] == "testValue1");
	check(tcr.getHeaders()[0]->getValue()[1] == "testValue2");
	// check(tcr.getHeaders().size() == 1);

	check(tcr.getQuery() == "testQuery");
	check(tcr.getPath() == "testPath");
	check(tcr.getFragment() == "testFragment");


}