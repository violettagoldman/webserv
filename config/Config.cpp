/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:43 by ashishae          #+#    #+#             */
/*   Updated: 2020/12/17 17:58:16 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{
	listenIp = 80;
	serverName.push_back("localhost");
	Location default_location;
	locations.push_back(default_location);
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

