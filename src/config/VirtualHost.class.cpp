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

VirtualHost::VirtualHost(ConfigFile &confFile) : 
	ABlock(confFile),
	serverName(1, ""),
	listenHost("*"),
	listenIp(80)
{
}

VirtualHost::VirtualHost(ABlock &ab) :
	ABlock(ab),
	serverName(1, ""),
	listenHost("*"),
	listenIp(80)
{
}

void VirtualHost::handleLine(std::string lineString)
{
	// std::cout << "VirtualHost handled: " << lineString << std::endl;
	if (lineString.find("location") != std::string::npos)
	{
		Location locBlock(*this);

		locBlock.setUploadStore(this->uploadStore);

		// locBlock.inheritParams(this->clientMaxBodySize, this->autoindex,
			// this->root, this->index, this->uploadStore);

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
	else if (isPresent(lineString, "upload_store"))
	{
		this->uploadStore = getStringDirective(lineString, "upload_store");
	}
	// else if (isPresent(lineString, "root"))
	// {
	// 	this->root = getStringDirective(lineString, "root");
	// }
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

std::string VirtualHost::getUploadStore(void) const
{
	return this->uploadStore;
}

void VirtualHost::check(void)
{
	// specific checks
	if (locations.size() == 0 && this->getRoot() == "" && this->getUploadStore() == "")
		throw Exception("VirtualHost has to either have a location, or specify root or upload.");
	for (size_t i = 0; i < locations.size(); i++)
	{
		locations[i].check();
	}
}