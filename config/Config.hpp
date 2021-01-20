/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:33 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/20 17:53:02 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>
# include <string>
# include <iostream>
# include <exception>
# include <ctype.h>

# include "Location.hpp"

// Global configuration object

// TODO: http context
// TODO: limit client body size in http context


// TODO: http methods for route
// TODO: default file (index)
// TODO: cgi

typedef struct sConfigPrototype
{
	int listenIp;
	std::string listenHost;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
	int clientMaxBodySize;
	bool autoindex;
}	configPrototype;

class Config {

public:
	Config(int listenIp, std::string listenHost, std::vector<std::string> serverName,
		std::vector<Location> locations, int clientMaxBodySize, bool autoindex);
	Config(configPrototype cp);
	// Config(std::vector<std::string> configBlock);
	// Config();
	~Config();
	// Пока сойдут implicit-версии
	// Config(const Config &copy);
	// Config &operator= (const Config &operand);
	int getListenIp(void) const;
	std::string getListenHost(void) const;
	std::vector<std::string> getServerName(void) const;
	std::vector<Location> getLocations(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;


	

private:
	int listenIp;
	int clientMaxBodySize;
	bool autoindex;
	std::string listenHost;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
};

#endif