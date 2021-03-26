/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.class.hpp"

Config::Config(ConfigFile &confFile) : ABlock(confFile)
{
}

/*
** In our implementation Config is the whole file (it's not wrapped
** in braces), so we don't need a break condition. We also don't need
** to check if the block was closed.
*/

void Config::handle()
{
	do
	{
		std::string ls = getConfFile().getLineString();
		checkLine(ls);
		handleLineCommon(ls);
		handleLine(ls);

	}
	while(getConfFile().getNext());
	Config::check();
}

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

void Config::check()
{
	for (size_t i = 0; i < virtualHostVector.size() - 1; i++)
	{
		if (virtualHostsHaveSameListen(virtualHostVector[i],
			virtualHostVector[i+1]) && duplicatesInServerName(
			virtualHostVector[i], virtualHostVector[i+1]))
		{
			throw Exception("Two servers with one server_name and listen");
		}
			
	}

	for (size_t i = 0; i < virtualHostVector.size(); i++)
	{
		virtualHostVector[i].check();
	}

}

void Config::handleLine(std::string lineString)
{
	// std::cout << "Config handled: " << lineString << std::endl;

	if (lineString.find("server") != std::string::npos)
	{
		// std::cout << "Found server" << std::endl;
		// VirtualHost sBlock(this->getConfFile());
		VirtualHost sBlock(*this);

		// sBlock.inheritParams(this->clientMaxBodySize, this->autoindex,
			// this->root, this->index);
		sBlock.handle();

		virtualHostVector.push_back(sBlock);
	}
	// else if (isPresent(lineString, "client_max_body_size"))
	// {
	// 	this->clientMaxBodySize = parseClientMaxBodySize(lineString);
	// }
	// else if (isPresent(lineString, "autoindex"))
	// {
	// 	this->autoindex = parseBoolDirective(lineString, "autoindex");
	// }
	// else if (isPresent(lineString, "index"))
	// {
	// 	this->index = ft_split(getStringDirective(lineString, "index"), ' ');
	// }
	// else if (isPresent(lineString, "root"))
	// {
	// 	this->root = getStringDirective(lineString, "root");
	// }
}

std::vector<VirtualHost> Config::getVirtualHostVector(void) const
{
	return this->virtualHostVector;
}

// int Config::getClientMaxBodySize(void) const
// {
// 	return this->clientMaxBodySize;
// }

// bool Config::getAutoindex(void) const
// {
// 	return this->autoindex;
// }

// std::vector<std::string> Config::getIndex(void) const
// {
// 	return this->index;
// }

// std::string Config::getRoot(void) const
// {
// 	return this->root;
// }




