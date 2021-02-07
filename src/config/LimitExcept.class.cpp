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