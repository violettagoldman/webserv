/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:43 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/20 19:46:06 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualHost.class.hpp"


VirtualHost::VirtualHost(int _listenIp, std::string _listenHost,
				std::vector<std::string> _serverName,
				std::vector<Location> _locations,
				int _clientMaxBodySize, bool _autoindex)
{
	listenIp = _listenIp;
	listenHost = _listenHost;
	serverName = _serverName;
	locations = _locations;
	clientMaxBodySize = _clientMaxBodySize;
	autoindex = _autoindex;

}

VirtualHost::VirtualHost(virtualHostPrototype cp)
{
	listenIp = cp.listenIp;
	listenHost = cp.listenHost;
	serverName = cp.serverName;
	locations = cp.locations;
	clientMaxBodySize = cp.clientMaxBodySize;
	autoindex = cp.autoindex;
	index = cp.index;
}

VirtualHost::~VirtualHost()
{
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

// VirtualHost::VirtualHost(const VirtualHost &copy)
// {
	
// }

// VirtualHost::VirtualHost &operator= (const VirtualHost &operand)
// {
	
// }

