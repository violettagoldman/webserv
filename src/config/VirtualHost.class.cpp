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

/*
** Constructor for VirtualHost.
*/
VirtualHost::VirtualHost(ConfigFile &confFile) : 
	ABlock(confFile),
	_listenPort(80),
	_listenHost("*"),
	_serverName(1, "")
{
}

/*
** Constructor for VirtualHost that inherits the directive values from whatever
** block is passed as a parameter.
*/
VirtualHost::VirtualHost(ABlock &ab) :
	ABlock(ab),
	_listenPort(80),
	_listenHost("*"),
	_serverName(1, "")
{
}

/*
** Handle a line that belongs to the block.
*/
void VirtualHost::handleLine(std::string lineString)
{
	if (lineString.find("location") != std::string::npos)
	{
		Location locBlock(*this);

		locBlock.setUploadStore(this->_uploadStore);
		locBlock.handle();

		_locations.push_back(locBlock);
	}

	if (isPresent(lineString, "listen"))
	{
		parseListen(lineString, this->_listenHost, this->_listenPort);
	}
	else if (isPresent(lineString, "server_name"))
	{
		this->_serverName = ft_split(getStringDirective(lineString,
										"server_name"), ' ');
	}
	else if (isPresent(lineString, "upload_store"))
	{
		this->_uploadStore = getStringDirective(lineString, "upload_store");
	}
}

/*
** Legacy Getter for _listenPort.
*/
int VirtualHost::getListenIp(void) const
{
	return this->_listenPort;
}

/*
** Getter for _listenPort.
*/
int VirtualHost::getListenPort(void) const
{
	return this->_listenPort;
}

/*
** Getter for _listenHost.
*/
std::string VirtualHost::getListenHost(void) const
{
	return this->_listenHost;
}

/*
** Getter for _serverName.
*/
std::vector<std::string> VirtualHost::getServerName(void) const
{
	return this->_serverName;
}

/*
** Getter for _locations.
*/
std::vector<Location> VirtualHost::getLocations(void) const
{
	return this->_locations;
}

/*
** Getter for uploadStore.
*/
std::string VirtualHost::getUploadStore(void) const
{
	return this->_uploadStore;
}

/*
** Check if virtualHost is well formed.
*/
void VirtualHost::check(void)
{
	// specific checks
	if (_locations.size() == 0 && this->getRoot() == "" && this->getUploadStore() == "")
		throw Exception("VirtualHost has to either have a location, or specify root or upload.");
	for (size_t i = 0; i < _locations.size(); i++)
	{
		_locations[i].check();
	}
}