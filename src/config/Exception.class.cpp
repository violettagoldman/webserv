/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 12:54:48 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/22 13:08:05 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exception.class.hpp"

Exception::Exception(std::string _message) :
	message(_message)
{
}

Exception::Exception(const Exception &copy) :
	message(copy.message)
{
}

Exception::~Exception()
{
}

Exception &Exception::operator= (const Exception &operand)
{
	if (this != &operand)
	{
		message = operand.message;
	}
	return (*this);
}

const char* Exception::what() const throw()
{
	return this->message.c_str();
}