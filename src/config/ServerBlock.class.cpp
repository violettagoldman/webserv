/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.class.hpp"

ServerBlock::ServerBlock(ConfigFile &confFile) : ABlock(confFile)
{
}

void ServerBlock::handleLine(std::string lineString)
{
	std::cout << "ServerBlock handled: " << lineString << std::endl;
	if (lineString.find("location") != std::string::npos)
	{
		LocationBlock locBlock(this->getConfFile());
		locBlock.handle();

		locations.push_back(locBlock);
	}
}

int ServerBlock::getListenIp(void) const
{
	return this->listenIp;
}

std::string ServerBlock::getListenHost(void) const
{
	return this->listenHost;
}

std::vector<std::string> ServerBlock::getServerName(void) const
{
	return this->serverName;
}

std::vector<LocationBlock> ServerBlock::getLocations(void) const
{
	return this->locations;
}

int ServerBlock::getClientMaxBodySize(void) const
{
	return this->clientMaxBodySize;
}

bool ServerBlock::getAutoindex(void) const
{
	return this->autoindex;
}

std::vector<std::string> ServerBlock::getIndex(void) const
{
	return this->index;
}

std::string ServerBlock::getUploadStore(void) const
{
	return this->uploadStore;
}

std::string ServerBlock::getRoot(void) const
{
	return this->root;
}