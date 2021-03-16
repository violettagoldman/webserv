/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_test_main.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 12:15:59 by ashishae          #+#    #+#             */
/*   Updated: 2021/03/16 15:04:34 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.class.hpp"
#include "TestCGIRequest.class.hpp"
#include "femtotest.hpp"
#include <cstdlib>
#include <map>
#include "Utility.hpp"


// Turn CGI reponse containing key:value pairs into a map for testing
std::map<std::string, std::string> responseToMap(std::string cgi_response)
{
	std::vector<std::string> lines = ft_split(cgi_response, '\n');

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

	return envVarMap;
}

// Write string to a special CGI and return what the Handler got back
std::string testBodyWrite(std::string stringToWrite,
							std::string s_pwd, std::string path_to_pcgi)
{
	CGIRequest cr3 = {
		"127.0.0.1", // remoteAddr
		// "testRemoteHost", // remoteHost
		"testAuthType", // authType
		"testRemoteIdent", // remoteIdent
		"testRemoteUser", // remoteUser
		"testContentType", // contentType
		"examplePathInfo", // pathInfo,
		"examplePathTranslated", // pathTranslated,
		"exampleQueryString", // queryString
		"GET", // requestMethod
		"http://example.com/cgi/test.php", // requestURI
		"80", // serverPort
		"example.com", // serverName
		s_pwd + "/cgi/test-cgi-body.php", // scriptFilename
		path_to_pcgi
	};

	CGIHandler h3(stringToWrite, cr3);

	std::string resp3 = h3.getCgiResponse();

	// Cut off the response headers and stuff
	size_t bodyIndex = resp3.find("BODY: ") + 6;

	return resp3.substr(bodyIndex, resp3.size() - bodyIndex);
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

	const char *logname = std::getenv("LOGNAME");

	std::string path_to_pcgi;

	// If we're at campus, php-cgi will be installed at brew
	if (logname != NULL)
		path_to_pcgi = "/Users/" + std::string(logname) + "/.brew/bin/php-cgi";
	else
		path_to_pcgi = "/usr/local/bin/php-cgi";

	std::cout << "Calculated php-cgi to be at: " << path_to_pcgi << std::endl;

	out("CGI Variables test");

	CGIRequest cr2 = {
		"127.0.0.1", // remoteAddr
		// "testRemoteHost", // remoteHost
		"testAuthType", // authType
		"testRemoteIdent", // remoteIdent
		"testRemoteUser", // remoteUser
		"testContentType", // contentType,
		"examplePathInfo", // pathInfo,
		"examplePathTranslated", // pathTranslated,
		"exampleQueryString", // queryString
		"GET", // requestMethod
		"http://example.com/cgi/test.php", // requestURI
		"80", // serverPort
		"example.com", // serverName
		s_pwd + "/cgi/test-cgi.php", // scriptFilename
		path_to_pcgi
	};

	CGIHandler h2("", cr2);

	std::string resp2 = h2.getCgiResponse();

	std::map<std::string, std::string> envVarMap = responseToMap(resp2);

	check(envVarMap["REMOTE_ADDR"] == "127.0.0.1"); 
	// check(envVarMap["REMOTE_HOST"] == "testRemoteHost");
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
	check(envVarMap["PATH_INFO"] == "examplePathInfo");
	check(envVarMap["GATEWAY_INTERFACE"] == "CGI/1.1");
	check(envVarMap["SERVER_PROTOCOL"] == "HTTP/1.1");
	check(envVarMap["SERVER_SOFTWARE"] == "Webserv/1.1");

	out("ft_itostr");

	out("ft_itostr | zero");
	check(ft_itostr(0) == "0");

	out("ft_itostr | positive number");
	check(ft_itostr(42) == "42");
	check(ft_itostr(100) == "100");
	check(ft_itostr(99) == "99");
	
	out("ft_itostr | negative number");
	check(ft_itostr(-42) == "-42");
	check(ft_itostr(-100) == "-100");
	check(ft_itostr(-99) == "-99");

	out("ft_itostr | maxint");
	check(ft_itostr(2147483647) == "2147483647");

	out("ft_itostr | minint");
	check(ft_itostr(-2147483648) == "-2147483648");



	out("CGI Body");

	check(testBodyWrite("testBody", s_pwd, path_to_pcgi) == "testBody\n");
	check(testBodyWrite("testBody\nlol", s_pwd, path_to_pcgi) == "testBody\nlol\n");
	check(testBodyWrite("testBody\nlol\nonemoreline", s_pwd, path_to_pcgi) == "testBody\nlol\nonemoreline\n");


	out("TestCGIRequest | Constructor");

	std::vector<Header> hds;

	std::vector<std::string> values;
	values.push_back("testValue1");
	values.push_back("testValue2");

	Header header = Header("testKey", values);

	hds.push_back(header);

	TestCGIRequest tcr("GET",
		"testBody",
		hds,
		// "testQuery",
		"testPath"
		// "testFragment"
	);

	check(tcr.getMethod() == "GET");
	check(tcr.getBody() == "testBody");

	check(tcr.getHeaders().size() == 1);
	check(tcr.getHeaders()[0].getName() == "testKey");
	check(tcr.getHeaders()[0].getValue().size() == 2);
	check(tcr.getHeaders()[0].getValue()[0] == "testValue1");
	check(tcr.getHeaders()[0].getValue()[1] == "testValue2");

	// check(tcr.getQuery() == "testQuery");
	check(tcr.getPath() == "testPath");
	// check(tcr.getFragment() == "testFragment");

	out("CGIHandler from Request and a few params");

	CGIRequires crs = {
		s_pwd + "/cgi/test-cgi.php", // scriptFilename
		"127.0.0.1", // remoteAddr
		"http://example.com/cgi/test.php", // requestURI
		"80", // serverPort
		"example.com", // serverName
		path_to_pcgi // pathToCGI
	};



	CGIHandler h3(tcr, crs);


	// std::cout << "Lsls" << std::endl;
	std::string resp3 = h3.getCgiResponse();
	// std::cout << resp3 << std::endl;

	envVarMap = responseToMap(resp3);

	check(envVarMap["REMOTE_ADDR"] == "127.0.0.1"); 
	// check(envVarMap["REMOTE_HOST"] == "testRemoteHost");
	// check(envVarMap["REMOTE_IDENT"] == "testRemoteIdent");
	// check(envVarMap["REMOTE_USER"] == "testRemoteUser");
	// check(envVarMap["AUTH_TYPE"] == "testAuthType");
	// check(envVarMap["CONTENT_TYPE"] == "testContentType");
	// check(envVarMap["REQUEST_METHOD"] == "GET");
	check(envVarMap["REQUEST_URI"] == "http://example.com/cgi/test.php");
	check(envVarMap["SERVER_PORT"] == "80");
	check(envVarMap["SERVER_NAME"] == "example.com");
	check(envVarMap["SCRIPT_FILENAME"] != "");
	check(envVarMap["SCRIPT_FILENAME"].find("test-cgi.php") != std::string::npos);
	
	check(envVarMap["SCRIPT_NAME"].find("test-cgi.php") != std::string::npos);
	
	// check(envVarMap["PATH_INFO"] == "examplePathInfo"); 
	
	check(envVarMap["GATEWAY_INTERFACE"] == "CGI/1.1");
	check(envVarMap["SERVER_PROTOCOL"] == "HTTP/1.1");
	check(envVarMap["SERVER_SOFTWARE"] == "Webserv/1.1");

	// std::cout <<  << std::endl;

	test_results();

}