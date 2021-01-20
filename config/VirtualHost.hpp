/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:33 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/20 19:35:31 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALHOST_HPP
# define VIRTUALHOST_HPP

# include <vector>
# include <string>
# include <iostream>
# include <exception>
# include <ctype.h>

# include "Location.hpp"

// Global VirtualHost object

// TODO: http methods for route
// TODO: default file (index)
// TODO: cgi
// TODO: client body size units

typedef struct sVirtualHostPrototype
{
	int listenIp;
	std::string listenHost;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
	int clientMaxBodySize;
	bool autoindex;
	std::vector<std::string> index;
}	virtualHostPrototype;

class VirtualHost {

public:
	VirtualHost(int listenIp, std::string listenHost, std::vector<std::string> serverName,
		std::vector<Location> locations, int clientMaxBodySize, bool autoindex);
	VirtualHost(virtualHostPrototype cp);
	// VirtualHost(std::vector<std::string> VirtualHostBlock);
	// VirtualHost();
	~VirtualHost();
	// Пока сойдут implicit-версии
	// VirtualHost(const VirtualHost &copy);
	// VirtualHost &operator= (const VirtualHost &operand);
	int getListenIp(void) const;
	std::string getListenHost(void) const;
	std::vector<std::string> getServerName(void) const;
	std::vector<Location> getLocations(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;
	std::vector<std::string> getIndex(void) const;


	

private:
	int listenIp;
	int clientMaxBodySize;
	bool autoindex;
	std::string listenHost;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
	std::vector<std::string> index;
};

#endif