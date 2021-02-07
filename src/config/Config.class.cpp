/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 18:40:56 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/20 19:36:44 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.class.hpp"

Config::Config(configPrototype cop)
{
	virtualHostVector = cop.virtualHostVector;
	clientMaxBodySize = cop.clientMaxBodySize;
	autoindex = cop.autoindex;
	index = cop.index;
}

std::vector<VirtualHost> Config::getVirtualHostVector(void) const
{
	return this->virtualHostVector;
}

int Config::getClientMaxBodySize(void) const
{
	return this->clientMaxBodySize;
}

bool Config::getAutoindex(void) const
{
	return this->autoindex;
}

std::vector<std::string> Config::getIndex(void) const
{
	return this->index;
}