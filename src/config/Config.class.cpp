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

bool duplicatesInServerName(VirtualHost vh1, VirtualHost vh2)
{
	std::vector<std::string> v1 = vh1.getServerName();
	std::vector<std::string> v2 = vh2.getServerName();

	for (size_t i = 0; i < v1.size(); ++i)
	{
		for (size_t j = 0; j < v2.size(); j++)
		{
			if (v1[i] == v2[j])
				return true;
		}
	}
	return false;
}

bool virtualHostsHaveSameListen(VirtualHost vh1, VirtualHost vh2)
{
	return (vh1.getListenIp() == vh2.getListenIp() &&\
			vh1.getListenHost() == vh2.getListenHost());
}


Config::Config(configPrototype cop)
{
	virtualHostVector = cop.virtualHostVector;

	for (size_t i = 0; i < virtualHostVector.size() - 1; i++)
	{
		if (virtualHostsHaveSameListen(virtualHostVector[i],
			virtualHostVector[i+1]) && duplicatesInServerName(
			virtualHostVector[i], virtualHostVector[i+1]))
			throw Exception("Two servers with one server_name and listen");
	}

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