/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   femtotest.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 12:17:05 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/31 12:19:04 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FEMTOTEST_HPP
# define FEMTOTEST_HPP

// Test-related stuff below

// Needed to automatically fail tests;
# include <assert.h>
# include <string>

# include <iostream>

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
		std::cout << e.what() << std::endl;\
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

template <typename T>
void check_eq(T a, T b)
{
	if (a == b)
	{
		std::cout << "\033[92m✓ PASS\033[0m" << std::endl;
	}
	else
	{
		std::cout << "\033[91m✓ FAIL\033[0m | " << "Expected: " << a << ", but got: " << b << std::endl;
	}
}

#endif
