/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:47:52 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/13 19:17:26 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

// Location::Location()
// {
// 	pattern = "/";
// 	root = "/var/www/";
// }

// /*
// ** Construct a Location with action passed as action_type (must be supported).
// ** If action is not supported, throw an exception.
// ** Set a named parameter to the passed value.
// ** @param _pattern A pattern for the location to match.
// ** @param action_type A string containing the name of the action
// **		("root" for example). Has to be supported.
// ** @param value The value for the action ("/var/www" for example)
// */
Location::Location(std::string _pattern, std::string action_type, std::string value)
{
	if (action_type == "root")
	{
		this->pattern = _pattern;
		this->root = value;
	}
	else
		throw 42;
}

/*
** This exception should be thrown where a config omits a required
** directive, for example, serverName or listen.
** Its functionality may be extended to specify the context and the 
** directive that is missing, as well as the line number.
** @ret Exception description.
*/
const char* Location::DirectiveNotFound::what() const throw()
{
	return "A required directive wasn't found in a context.";
}

/*
** Read the captured locationBlock into a Location object. If the block
** doesn't contain a valid action, throw an exception.
** @param lb The locationBlock object that contains the captured location.
** @ret Location The resulting Location object.
*/
Location::Location(std::string pattern, std::string block)
{
	// DirectiveMap dm(block);
	// dirmap directives = dm.getMap();
	std::string tmp;
	// std::cout << "Block: " << std::endl;
	// for (int i = 0; i < block.size(); i++)
	// {
	// 	std::cout << block[i] << std::endl;
	// }

		// return Location(lb.pattern, "root", tmp);

	this->pattern = pattern;
	this->root = read_directive(block, "root");
	
}

Location::~Location()
{
}

std::string Location::getPattern(void) const
{
	return this->pattern;
}

std::string Location::getRoot(void) const
{
	return this->root;
}


// Location::Location(const Location &copy)
// {
	
// }

// Location::Location &operator= (const Location &operand)
// {
	

// }

