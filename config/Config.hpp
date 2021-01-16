/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:33 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/16 15:49:17 by ashishae         ###   ########.fr       */
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
// TODO: directory listing on/off (autoindex)
// TODO: default file (index)
// TODO: cgi

class Config {

public:
	Config(int listenIp, std::string listenHost, std::vector<std::string> serverName,
		std::vector<Location> locations, int clientMaxBodySize);
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


	

private:
	int listenIp;
	int clientMaxBodySize;
	std::string listenHost;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
};

#endif