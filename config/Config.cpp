/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:43 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/20 17:33:29 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"


Config::Config(int _listenIp, std::string _listenHost,
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

Config::Config(configPrototype cp)
{
	listenIp = cp.listenIp;
	listenHost = cp.listenHost;
	serverName = cp.serverName;
	locations = cp.locations;
	clientMaxBodySize = cp.clientMaxBodySize;
	autoindex = cp.autoindex;
}

Config::~Config()
{
}

int Config::getListenIp(void) const
{
	return this->listenIp;
}

std::string Config::getListenHost(void) const
{
	return this->listenHost;
}

std::vector<std::string> Config::getServerName(void) const
{
	return this->serverName;
}

std::vector<Location> Config::getLocations(void) const
{
	return this->locations;
}

int Config::getClientMaxBodySize(void) const
{
	return this->clientMaxBodySize;
}

bool Config::getAutoindex(void) const
{
	return this->autoindex;
}

// Config::Config(const Config &copy)
// {
	
// }

// Config::Config &operator= (const Config &operand)
// {
	
// }

