/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualHost.class.hpp"

VirtualHost::VirtualHost(ConfigFile &confFile) : ABlock(confFile)
{
}

void VirtualHost::handleLine(std::string lineString)
{
	// std::cout << "VirtualHost handled: " << lineString << std::endl;
	if (lineString.find("location") != std::string::npos)
	{
		Location locBlock(this->getConfFile());

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

void VirtualHost::inheritParams(int _clientMaxBodySize, bool _autoindex,
		std::string _root, std::vector<std::string> _index)
{
	this->clientMaxBodySize = _clientMaxBodySize;
	this->autoindex = _autoindex;
	this->root = _root;
	this->index = _index;
}

int VirtualHost::getListenIp(void) const
{
	return this->listenIp;
}

std::string VirtualHost::getListenHost(void) const
{
	return this->listenHost;
}

std::vector<std::string> VirtualHost::getServerName(void) const
{
	return this->serverName;
}

std::vector<Location> VirtualHost::getLocations(void) const
{
	return this->locations;
}

int VirtualHost::getClientMaxBodySize(void) const
{
	return this->clientMaxBodySize;
}

bool VirtualHost::getAutoindex(void) const
{
	return this->autoindex;
}

std::vector<std::string> VirtualHost::getIndex(void) const
{
	return this->index;
}

std::string VirtualHost::getUploadStore(void) const
{
	return this->uploadStore;
}

std::string VirtualHost::getRoot(void) const
{
	return this->root;
}

void VirtualHost::check(void)
{
	// specific checks
	for (size_t i = 0; i < locations.size(); i++)
	{
		locations[i].check();
	}
}