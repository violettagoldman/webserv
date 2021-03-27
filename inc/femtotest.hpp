/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   femtotest.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 12:17:05 by ashishae          #+#    #+#             */
/*   Updated: 2021/03/02 13:20:49 by ashishae         ###   ########.fr       */
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

int total_checks = 0;
int tests_passed = 0;

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
		bool messageEqual = !strcmp(e.what(), exceptionString); \
		check(messageEqual == true); \
		if (!messageEqual) \
		{ \
			std::cout << "Expected exception: " << exceptionString \
				<< ", but got: " << e.what() << std::endl;\
		} \
		std::cout << e.what() << std::endl;\
	} \
	check(exception_thrown == true); \
	if (!exception_thrown)\
	{\
		std::cout << "Exception not thrown when expected" << std::endl;\
	}\
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
	total_checks += 1;
	if (expression == 1)
	{
		std::cout << "\033[92m✓ PASS\033[0m" << std::endl;
		tests_passed += 1;
	}
	else 
	{
		std::cout << "\033[91m✓ FAIL\033[0m" << std::endl;
	}
	
}

void test_results()
{
	std::cout << "\n\n---------" << std::endl;
	std::cout << "Total checks: " << total_checks << ", passed: " << tests_passed << ", failed: " << total_checks - tests_passed << std::endl;
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
