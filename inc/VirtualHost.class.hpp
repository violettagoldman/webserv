/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:33 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/07 15:52:53 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALHOST_CLASS_HPP
# define VIRTUALHOST_CLASS_HPP

# include <vector>
# include <string>

# include "Location.class.hpp"

// Global VirtualHost object

// TODO: atoi

// TODO: exceptions

typedef struct sVirtualHostPrototype
{
	int listenIp;
	std::string listenHost;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
	int clientMaxBodySize;
	bool autoindex;
	std::vector<std::string> index;
	std::string uploadStore;
	std::string root;
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
	std::string getUploadStore(void) const;
	std::string getRoot(void) const;

private:
	int listenIp;
	int clientMaxBodySize;
	bool autoindex;
	std::string listenHost;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
	std::vector<std::string> index;
	std::string uploadStore;
	std::string root;
};

#endif