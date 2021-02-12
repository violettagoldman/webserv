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
	// std::cout << "ServerBlock handled: " << lineString << std::endl;
	if (lineString.find("location") != std::string::npos)
	{
		LocationBlock locBlock(this->getConfFile());

		locBlock.inheritParams(this->clientMaxBodySize, this->autoindex,
			this->root, this->index, this->uploadStore);

		locBlock.handle();

		locations.push_back(locBlock);
	}

	if (isPresent(lineString, "listen"))
	{
		parseListen(lineString, this->listenHost, this->listenIp);
	}
	else if (isPresent(lineString, "server_name"))
	{
		this->serverName = ft_split(getStringDirective(lineString,
										"server_name"), ' ');
	}
	else if (isPresent(lineString, "client_max_body_size"))
	{
		this->clientMaxBodySize = parseClientMaxBodySize(lineString);
	}
	else if (isPresent(lineString, "autoindex"))
	{
		this->autoindex = parseBoolDirective(lineString, "autoindex");
	}
	else if (isPresent(lineString, "index"))
	{
		this->index = ft_split(getStringDirective(lineString, "index"), ' ');
	}
	else if (isPresent(lineString, "upload_store"))
	{
		this->uploadStore = getStringDirective(lineString, "upload_store");
	}
	else if (isPresent(lineString, "root"))
	{
		this->root = getStringDirective(lineString, "root");
	}
}

void ServerBlock::inheritParams(int _clientMaxBodySize, bool _autoindex,
		std::string _root, std::vector<std::string> _index)
{
	this->clientMaxBodySize = _clientMaxBodySize;
	this->autoindex = _autoindex;
	this->root = _root;
	this->index = _index;
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