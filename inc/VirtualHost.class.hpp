/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/03/27 13:51:56 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALHOST_CLASS_HPP
# define VIRTUALHOST_CLASS_HPP

# include "ABlock.class.hpp"
# include "Location.class.hpp"

/*
** The VirtualHost class represents a server-block in an NGINX config file.
** The name, ironically, comes from Apache. But we already had a Server
** class at that moment.
*/
class VirtualHost : public ABlock
{
public:
	VirtualHost(ConfigFile &confFile);

	VirtualHost(ABlock &ab);
	void handleLine(std::string lineString);

	int getListenIp(void) const;
	int getListenPort(void) const;
	std::string getListenHost(void) const;
	std::vector<std::string> getServerName(void) const;
	std::vector<Location> getLocations(void) const;
	std::string getUploadStore(void) const;

	void check(void);

private:

	int _listenPort;
	std::string _listenHost;
	std::vector<std::string> _serverName;
	std::vector<Location> _locations;
	std::string _uploadStore;
};

#endif