/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_tests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:45:02 by ashishae          #+#    #+#             */
/*   Updated: 2020/12/17 18:01:05 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Test-related stuff below

// Needed to automatically fail tests;
#include <assert.h>
#include <string>

#include <iostream>

bool exception_thrown = false;

void check(int expression);

#define TEST_EXCEPTION(expression, exceptionType) { \
	exception_thrown = false; \
	try \
	{ \
		expression; \
	} \
	catch (const exceptionType &e) \
	{ \
		exception_thrown = true; \
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
	assert(expression == 1);
	std::cout << "\033[92m✓ PASS\033[0m" << std::endl;
}

#include "Config.hpp"

int main(void)
{
	out("Config | Default config");
	Config default_config;

	check(default_config.getListenIp() == 80);
	check(default_config.getServerName().size() == 1);
	check(default_config.getServerName()[0] == "localhost");
	check(default_config.getLocations().size() == 1);
	check(default_config.getLocations()[0].getPattern() == "/");
	check(default_config.getLocations()[0].getRoot() == "/var/www/");
	
}