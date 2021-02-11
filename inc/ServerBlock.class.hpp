/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_CLASS_HPP
# define SERVERBLOCK_CLASS_HPP

# include "ABlock.class.hpp"
# include "LocationBlock.class.hpp"

class ServerBlock : public ABlock
{
public:
	ServerBlock(ConfigFile &confFile);
	virtual void handleLine(std::string lineString);

	int getListenIp(void) const;
	std::string getListenHost(void) const;
	std::vector<std::string> getServerName(void) const;
	std::vector<LocationBlock> getLocations(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;
	std::vector<std::string> getIndex(void) const;
	std::string getUploadStore(void) const;
	std::string getRoot(void) const;

private:
	std::vector<LocationBlock> locationBlockVector;



	int listenIp;
	int clientMaxBodySize;
	bool autoindex;
	std::string listenHost;
	std::vector<std::string> serverName;
	std::vector<LocationBlock> locations;
	std::vector<std::string> index;
	std::string uploadStore;
	std::string root;

};

#endif