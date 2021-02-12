/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptBlock.class.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExceptBlock.class.hpp"

LimitExceptBlock::LimitExceptBlock(ConfigFile &confFile) : ABlock(confFile)
{
}

std::string LimitExceptBlock::parseMethod(std::string line)
{
	size_t loc_position = line.find("limit_except");
	size_t pattern_start = loc_position + 13;
	size_t pattern_end = line.find("{", pattern_start);
	std::string pattern = line.substr(pattern_start, pattern_end-pattern_start);
	trimWhitespace(pattern);
	return pattern;
}

void LimitExceptBlock::handleLine(std::string lineString)
{
	// std::cout << "LimitExceptBlock handled: " << lineString << std::endl;

	std::vector<std::string> appendix;

	if (isPresent(lineString, "limit_except"))
	{
		this->method = parseMethod(lineString);
	}
	if (isPresent(lineString, "allow"))
	{
		appendix = ft_split(getStringDirective(lineString, "allow"), ' ');
		this->allow.insert(this->allow.end(), appendix.begin(), appendix.end());
	}
	if (isPresent(lineString, "deny"))
	{
		appendix = ft_split(getStringDirective(lineString, "deny"), ' ');
		this->deny.insert(this->deny.end(), appendix.begin(), appendix.end());
	}
}


std::vector<std::string> LimitExceptBlock::getAllow(void) const
{
	return allow;
}

std::vector<std::string> LimitExceptBlock::getDeny(void) const
{
	return deny;
}

std::string LimitExceptBlock::getMethod(void) const
{
	return method;
}