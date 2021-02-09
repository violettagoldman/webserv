/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:47:52 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/22 11:19:16 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.class.hpp"

// /*
// ** Construct a Location with action passed as action_type (must be supported).
// ** If action is not supported, throw an exception.
// ** Set a named parameter to the passed value.
// ** @param _pattern A pattern for the location to match.
// ** @param action_type A string containing the name of the action
// **		("root" for example). Has to be supported.
// ** @param value The value for the action ("/var/www" for example)
// */
// Location::Location(std::string _pattern, std::string action_type,
// 	std::string value, int _clientMaxBodySize, bool _autoindex)
// {
// 	if (action_type == "root")
// 	{
// 		this->pattern = _pattern;
// 		this->root = value;
// 		this->clientMaxBodySize = _clientMaxBodySize;
// 		this->autoindex = _autoindex;
// 	}
// 	else
// 		throw 42;
// }

Location::Location(locationPrototype lp) :
	clientMaxBodySize(lp.clientMaxBodySize),
	autoindex(lp.autoindex),
	pattern(lp.pattern),
	root(lp.root),
	index(lp.index),
	limitExcept(lp.limitExcept),
	fcgiPass(lp.fcgiPass),
	fcgiParams(lp.fcgiParams),
	uploadStore(lp.uploadStore)
{
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

int Location::getClientMaxBodySize(void) const 
{
	return this->clientMaxBodySize;
}

bool Location::getAutoindex(void) const
{
	return this->autoindex;
}

std::vector<std::string> Location::getIndex(void) const
{
	return this->index;
}

LimitExcept Location::getLimitExcept(void) const
{
	return this->limitExcept;
}

std::string Location::getFcgiPass(void) const
{
	return this->fcgiPass;
}

std::map<std::string, std::string> Location::getFcgiParams(void) const
{
	return this->fcgiParams;
}

std::string Location::getUploadStore(void) const
{
	return this->uploadStore;
}
