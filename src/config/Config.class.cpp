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

Config::Config()
{
}

/*
** Config constructor.
*/
Config::Config(ConfigFile *confFile) : ABlock(confFile)
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
		std::string ls = getConfFile()->getLineString();
		checkLine(ls);
		handleLineCommon(ls);
		handleLine(ls);

	}
	while(getConfFile()->getNext());
	Config::check();
}

/*
** Return true, if two VirtualHosts have common serverNames.
** @param vh1 First VH.
** @param vh2 Second VH.
** @ret bool Do the VH have at least one common serverName.
*/
bool duplicatesInServerName(VirtualHost vh1, VirtualHost vh2)
{
	const std::vector<std::string> v1 = vh1.getServerName();
	const std::vector<std::string> v2 = vh2.getServerName();

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

/*
** Return true, if two VirtualHosts have the same listen directive.
** @param vh1 First VH.
** @param vh2 Second VH.
** @ret bool Do the VH have the same listen.
*/
bool virtualHostsHaveSameListen(VirtualHost vh1, VirtualHost vh2)
{
	return (vh1.getListenIp() == vh2.getListenIp() &&\
			vh1.getListenHost() == vh2.getListenHost());
}

/*
** Check if the Config object is completely formed, and raise necessary
** exceptions.
*/
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

/*
** Handle a line that belongs to Config.
*/
void Config::handleLine(std::string lineString)
{
	if (lineString.find("server") != std::string::npos)
	{
		VirtualHost sBlock(*this);
		sBlock.handle();

		virtualHostVector.push_back(sBlock);
	}
}

/*
** Getter for VirtualHost.
*/
std::vector<VirtualHost> Config::getVirtualHostVector(void) const
{
	return this->virtualHostVector;
}
