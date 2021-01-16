/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:43 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/16 11:34:27 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"


Config::Config(int _listenIp, std::vector<std::string> _serverName,
				std::vector<Location> _locations)
{
	listenIp = _listenIp;
	serverName = _serverName;
	locations = _locations;
}

Config::~Config()
{
}

int Config::getListenIp(void) const
{
	return this->listenIp;
}

std::vector<std::string> Config::getServerName(void) const
{
	return this->serverName;
}

std::vector<Location> Config::getLocations(void) const
{
	return this->locations;
}


// Config::Config(const Config &copy)
// {
	
// }

// Config::Config &operator= (const Config &operand)
// {
	
// }

