/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_tests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:45:02 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/16 15:26:41 by ashishae         ###   ########.fr       */
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

#include "Config.hpp"
#include "Reader.hpp"

int main(void)
{
	// out("Config | Default config");
	// Config default_config;
	Reader r("nginx.conf");

	std::vector<Config> configVector = r.getConfigVector();
	check(configVector.size() == 2);
	check(configVector[0].getListenIp() == 80);
	check(configVector[0].getListenHost() == "");
	check(configVector[0].getServerName().size() == 2);
	check(configVector[0].getServerName()[0] == "domain1.com");
	check(configVector[0].getServerName()[1] == "www.domain1.com");
	check(configVector[0].getLocations().size() == 1);
	check(configVector[0].getLocations()[0].getPattern() == "/");
	check(configVector[0].getLocations()[0].getRoot() == "/var/www/");
	check(configVector[1].getListenHost() == "127.0.0.1");
	check(configVector[1].getListenIp() == 443);
	check(configVector[1].getServerName().size() == 2);
	check(configVector[1].getServerName()[0] == "domain2.com");
	check(configVector[1].getServerName()[1] == "www.domain2.com");
	
	check(configVector[1].getLocations().size() == 1);

	check(configVector[1].getLocations()[0].getPattern() == "/app/");

	check(configVector[1].getLocations()[0].getRoot() == "/var/www/");

	// TEST_EXCEPTION(Reader r2("missing_listen.conf"), Config::DirectiveNotFound,\
	// 				"A required directive wasn't found in a context.");

	// TEST_EXCEPTION(Reader r2("missing_root.conf"), Location::DirectiveNotFound,\
	// 				"A required directive wasn't found in a context.");
	
	// TEST_EXCEPTION(Reader r2("syntaxerror.conf"), Reader::SyntaxError,\
	// 				"The config file contains a syntax error.");

					
	


	// check(default_config.getListenIp() == 80);
	// check(default_config.getServerName().size() == 1);
	// check(default_config.getServerName()[0] == "localhost");
	// check(default_config.getLocations().size() == 1);
	// check(default_config.getLocations()[0].getPattern() == "/");
	// check(default_config.getLocations()[0].getRoot() == "/var/www/");

	
	
}