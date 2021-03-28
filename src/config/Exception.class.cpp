/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.class.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 12:54:48 by ashishae          #+#    #+#             */
/*   Updated: 2021/03/27 17:07:56 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exception.class.hpp"

#include <iostream>

/*
** Constructor for Exception.
*/
Exception::Exception(std::string _message) :
	message(_message)
{
}

/*
** Copy constructor for Exception.
*/
Exception::Exception(const Exception &copy) :
	message(copy.message)
{
}

/*
** Destructor for Exception.
*/
Exception::~Exception()
{
}

/*
** Assignment ioperator overload for Exception.
*/
Exception &Exception::operator= (const Exception &operand)
{
	if (this != &operand)
	{
		message = operand.message;
	}
	return (*this);
}

/*
** What() function for reading an exception message.
*/
const char* Exception::what() const throw()
{
	return this->message.c_str();
}