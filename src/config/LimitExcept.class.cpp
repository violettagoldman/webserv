/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.class.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExcept.class.hpp"

/*
** Constructor for LimitExcept.
*/
LimitExcept::LimitExcept(ConfigFile *confFile) : ABlock(confFile), _empty(true)
{
}

/*
** Parse the methods string in the limit_except directive.
** ex: limit_except GET POST {} will return {"GET", "POST"}
** @param line The first line of said directive.
** @ret vector<string> The parsed methods.
*/
std::vector<std::string> LimitExcept::parseMethods(std::string line)
{
	size_t loc_position = line.find("limit_except");
	size_t pattern_start = loc_position + 13;
	size_t pattern_end = line.find("{", pattern_start);
	std::string pattern = line.substr(pattern_start, pattern_end-pattern_start);
	trimWhitespace(pattern);
	trimWhitespaceStart(pattern);
	return ft_split(pattern, ' ');
}

/*
** Handle a line belonging to the LimitExcept block.
*/
void LimitExcept::handleLine(std::string lineString)
{
	std::vector<std::string> appendix;

	if (isPresent(lineString, "limit_except"))
	{
		this->_methods = parseMethods(lineString);
		this->_empty = false;
	}
	if (isPresent(lineString, "allow"))
	{
		appendix = ft_split(getStringDirective(lineString, "allow"), ' ');
		this->_allow.insert(this->_allow.end(), appendix.begin(), appendix.end());
		this->_empty = false;
	}
	if (isPresent(lineString, "deny"))
	{
		appendix = ft_split(getStringDirective(lineString, "deny"), ' ');
		this->_deny.insert(this->_deny.end(), appendix.begin(), appendix.end());
		this->_empty = false;
	}
}

/*
** Getter for _allow.
*/
std::vector<std::string> LimitExcept::getAllow(void) const
{
	return _allow;
}

/*
** Getter for _deny.
*/
std::vector<std::string> LimitExcept::getDeny(void) const
{
	return _deny;
}

/*
** Getter for _methods.
*/
std::vector<std::string> LimitExcept::getMethods(void) const
{
	return _methods;
}

/*
** Return false if limitExcept wasn't specified in the parent block, and 
** true if this block exists in the parent block.
** @ret bool ditto.
*/
bool LimitExcept::isEmpty(void) const
{
	return _empty;
}