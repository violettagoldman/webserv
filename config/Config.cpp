/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:43 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/16 15:42:33 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"


Config::Config(int _listenIp, std::string _listenHost, std::vector<std::string> _serverName,
				std::vector<Location> _locations, int _clientMaxBodySize)
{
	listenIp = _listenIp;
	listenHost = _listenHost;
	serverName = _serverName;
	locations = _locations;
	clientMaxBodySize = _clientMaxBodySize;
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

// Config::Config(const Config &copy)
// {
	
// }

// Config::Config &operator= (const Config &operand)
// {
	
// }

