/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 18:01:13 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/21 22:12:46 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExcept.class.hpp"

std::vector<std::string> ft_split(std::string s, char c)
{
	std::vector<std::string> ret;
	int i = 0;
	size_t pos;
	while ((pos = s.find(c, i)) != std::string::npos)
	{
		ret.push_back(s.substr(i, pos-i));
		i = pos+1;
	}
	ret.push_back(s.substr(i));
	return ret;
}

LimitExcept::LimitExcept()
{
}

LimitExcept::LimitExcept(std::vector<std::string> _allow,
	std::vector<std::string> _deny, std::string _method) :
	allow(_allow), deny(_deny), method(_method)
{
}

LimitExcept::LimitExcept(limitExceptPrototype lep):
	allow(lep.allow), deny(lep.deny), method(lep.method)
{
}

std::vector<std::string> LimitExcept::getAllow(void) const
{
	return allow;
}

std::vector<std::string> LimitExcept::getDeny(void) const
{
	return deny;
}

std::string LimitExcept::getMethod(void) const
{
	return method;
}