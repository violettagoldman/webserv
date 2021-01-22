/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_tests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:45:02 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/22 11:23:40 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Test-related stuff below

// Needed to automatically fail tests;
#include <assert.h>
#include <string>

#include <iostream>

bool exception_thrown = false;

void check(int expression);

#define TEST_EXCEPTION(expression, exceptionType, exceptionString) { \
	exception_thrown = false; \
	try \
	{ \
		expression; \
	} \
	catch (const exceptionType &e) \
	{ \
		exception_thrown = true; \
		check((!strcmp(e.what(), exceptionString))); \
	} \
	check(exception_thrown == true); \
}

int test_no = 1;

void out(std::string s)
{
	std::cout << std::endl;
	std::cout << "\033[0;34m" << "Test " << test_no << " | " << s << "\033[0m" << std::endl;
	test_no += 1;
}

void check(int expression)
{
	// If expression doesn't evaluate to 1, the program will abort
	// assert(expression == 1);
	if (expression == 1)
	{
		std::cout << "\033[92m✓ PASS\033[0m" << std::endl;
	}
	else 
	{
		std::cout << "\033[91m✓ FAIL\033[0m" << std::endl;
	}
	
}

#include "VirtualHost.hpp"
#include "Reader.hpp"

int main(void)
{
	// out("virtualHost | Default virtualHost");
	// virtualHost default_virtualHost;
	Reader r("nginx.conf");

	Config *conf = r.createConfig();

	check(conf->getAutoindex() == true);
	check(conf->getClientMaxBodySize() == 32);
	check(conf->getIndex().size() == 1);
	check(conf->getIndex()[0] == "index.html");

	std::vector<VirtualHost> virtualHostVector = conf->getVirtualHostVector();
	check(virtualHostVector.size() == 3);

	check(virtualHostVector[0].getListenIp() == 80);
	check(virtualHostVector[0].getListenHost() == "");
	check(virtualHostVector[0].getServerName().size() == 2);
	check(virtualHostVector[0].getServerName()[0] == "domain1.com");
	check(virtualHostVector[0].getServerName()[1] == "www.domain1.com");
	check(virtualHostVector[0].getAutoindex() == true);

	out("Host 0 | Index");
	check(virtualHostVector[0].getIndex().size() == 2);
	check(virtualHostVector[0].getIndex()[0] == "index.php");
	check(virtualHostVector[0].getIndex()[1] == "index2.php");

	check(virtualHostVector[0].getLocations().size() == 1);

	check(virtualHostVector[0].getLocations()[0].getPattern() == "/");
	check(virtualHostVector[0].getLocations()[0].getRoot() == "/var/www/");

	check(virtualHostVector[0].getClientMaxBodySize() == 32);



	check(virtualHostVector[1].getListenHost() == "127.0.0.1");
	check(virtualHostVector[1].getListenIp() == 443);
	check(virtualHostVector[1].getServerName().size() == 2);
	check(virtualHostVector[1].getServerName()[0] == "domain2.com");
	check(virtualHostVector[1].getServerName()[1] == "www.domain2.com");
	check(virtualHostVector[1].getClientMaxBodySize() == 1024);
	check(virtualHostVector[1].getAutoindex() == true);


	
	check(virtualHostVector[1].getLocations().size() == 1);

	check(virtualHostVector[1].getLocations()[0].getPattern() == "/app/");
	check(virtualHostVector[1].getLocations()[0].getAutoindex() == true);
	check(virtualHostVector[1].getLocations()[0].getClientMaxBodySize() == 1024);

	check(virtualHostVector[1].getLocations()[0].getRoot() == "/var/www/");
	out("Host 1 | Location 0 | limit_except ");
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getMethod() == "GET");
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getAllow().size() == 2);
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getAllow()[0] == "127.0.0.1");
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getAllow()[1] == "127.0.0.2");
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getDeny().size() == 1);
	// check(virtualHostVector[1].getLocations()[0].getLimitExcept().getDeny()[0] == "all");

	out("Host 1 | Location 0 | Index");
	check(virtualHostVector[1].getLocations()[0].getIndex().size() == 1);
	check(virtualHostVector[1].getLocations()[0].getIndex()[0] == "index3.html");

	check(virtualHostVector[2].getClientMaxBodySize() == 1024);
	check(virtualHostVector[2].getAutoindex() == true);
	check(virtualHostVector[2].getLocations()[0].getAutoindex() == true);

	out("Host 2 | Location 1 | fastcgi_pass");
	check(virtualHostVector[2].getLocations()[1].getFcgiPass() == "127.0.0.1:9000");
	check(virtualHostVector[2].getLocations()[1].getFcgiParams()["TEST_PARAM"] == "test_val");
	// TEST_EXCEPTION(Reader r2("missing_listen.conf"), virtualHost::DirectiveNotFound,\
	// 				"A required directive wasn't found in a context.");

	// TEST_EXCEPTION(Reader r2("missing_root.conf"), Location::DirectiveNotFound,\
	// 				"A required directive wasn't found in a context.");
	
	// TEST_EXCEPTION(Reader r2("syntaxerror.conf"), Reader::SyntaxError,\
	// 				"The virtualHost file contains a syntax error.");

					
	


	// check(default_virtualHost.getListenIp() == 80);
	// check(default_virtualHost.getServerName().size() == 1);
	// check(default_virtualHost.getServerName()[0] == "localhost");
	// check(default_virtualHost.getLocations().size() == 1);
	// check(default_virtualHost.getLocations()[0].getPattern() == "/");
	// check(default_virtualHost.getLocations()[0].getRoot() == "/var/www/");

	
	
}