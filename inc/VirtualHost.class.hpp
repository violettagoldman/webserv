/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALHOST_CLASS_HPP
# define VIRTUALHOST_CLASS_HPP

# include "ABlock.class.hpp"
# include "Location.class.hpp"

class VirtualHost : public ABlock
{
public:
	VirtualHost(ConfigFile &confFile);
	void handleLine(std::string lineString);

	int getListenIp(void) const;
	std::string getListenHost(void) const;
	std::vector<std::string> getServerName(void) const;
	std::vector<Location> getLocations(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;
	std::vector<std::string> getIndex(void) const;
	std::string getUploadStore(void) const;
	std::string getRoot(void) const;

	void inheritParams(int clientMaxBodySize, bool autoindex, std::string root,
			std::vector<std::string> index);

	void check(void);

private:
	std::vector<Location> locationBlockVector;

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