/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:33 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/16 13:21:10 by ashishae         ###   ########.fr       */
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

// TODO: listen host
// TODO: limit client body size
// TODO: http methods for route
// TODO: directory listing on/off (autoindex)
// TODO: default file (index)
// TODO: cgi

class Config {

public:
	Config(int listenIp, std::vector<std::string> serverName,
		std::vector<Location> locations);
	// Config(std::vector<std::string> configBlock);
	// Config();
	~Config();
	// Пока сойдут implicit-версии
	// Config(const Config &copy);
	// Config &operator= (const Config &operand);
	int getListenIp(void) const;
	std::vector<std::string> getServerName(void) const;
	std::vector<Location> getLocations(void) const;


	

private:
	int listenIp;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
};

#endif