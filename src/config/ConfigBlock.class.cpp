/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigBlock.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigBlock.class.hpp"

ConfigBlock::ConfigBlock(ConfigFile &confFile) : ABlock(confFile)
{
}

/*
** In our implementation ConfigBlock is the whole file (it's not wrapped
** in braces), so we don't need a break condition. We also don't need
** to check if the block was closed.
*/

void ConfigBlock::handle()
{
	do
	{
		checkLine(getConfFile().getLineString());
		handleLine(getConfFile().getLineString());

	}
	while(getConfFile().getNext());
	ConfigBlock::check();
}

bool duplicatesInServerName(ServerBlock vh1, ServerBlock vh2)
{
	std::vector<std::string> v1 = vh1.getServerName();
	std::vector<std::string> v2 = vh2.getServerName();

	for (size_t i = 0; i < v1.size(); ++i)
	{
		for (size_t j = 0; j < v2.size(); j++)
		{
			if (v1[i] == v2[j])
				return true;
		}
	}
	return false;
}

bool virtualHostsHaveSameListen(ServerBlock vh1, ServerBlock vh2)
{
	return (vh1.getListenIp() == vh2.getListenIp() &&\
			vh1.getListenHost() == vh2.getListenHost());
}

void ConfigBlock::check()
{
	for (size_t i = 0; i < virtualHostVector.size() - 1; i++)
	{
		if (virtualHostsHaveSameListen(virtualHostVector[i],
			virtualHostVector[i+1]) && duplicatesInServerName(
			virtualHostVector[i], virtualHostVector[i+1]))
		{
			throw Exception("Two servers with one server_name and listen");
		}
			
	}

}

void ConfigBlock::handleLine(std::string lineString)
{
	// std::cout << "ConfigBlock handled: " << lineString << std::endl;

	if (lineString.find("server") != std::string::npos)
	{
		// std::cout << "Found server" << std::endl;
		ServerBlock sBlock(this->getConfFile());

		sBlock.inheritParams(this->clientMaxBodySize, this->autoindex,
			this->root, this->index);
		sBlock.handle();

		virtualHostVector.push_back(sBlock);
	}
	else if (isPresent(lineString, "client_max_body_size"))
	{
		this->clientMaxBodySize = parseClientMaxBodySize(lineString);
	}
	else if (isPresent(lineString, "autoindex"))
	{
		this->autoindex = parseBoolDirective(lineString, "autoindex");
	}
	else if (isPresent(lineString, "index"))
	{
		this->index = ft_split(getStringDirective(lineString, "index"), ' ');
	}
	else if (isPresent(lineString, "root"))
	{
		this->root = getStringDirective(lineString, "root");
	}

	// if (isPresent(lineString, "root"))
	// {
	// 	this->root = getStringDirective(lineString, "root");
	// 	this->rootSet = true;
	// }
	// if (isPresent(lineString, "location"))
	// {
	// 	this->pattern = parsePattern(lineString);
	// }
	// if (isPresent(lineString, "client_max_body_size"))
	// {
	// 	this->clientMaxBodySize = parseClientMaxBodySize(lineString);
	// }
	// if (isPresent(lineString, "autoindex"))
	// {
	// 	this->autoindex = parseBoolDirective(getStringDirective(lineString, "autoindex"));
	// }
	// if (isPresent(lineString, "fastcgi_pass"))
	// {
	// 	this->fcgiPass = getStringDirective(lineString, "fastcgi_pass");
	// 	this->fcgiSet = true;
	// }
	// if (isPresent(lineString, "fastcgi_param"))
	// {
	// 	parseFastCGIParam(lineString, this->fcgiParams);
	// }
	// else if (isPresent(lineString, "index"))
	// {
	// 	this->index = ft_split(getStringDirective(lineString, "index"), ' ');
	// }
	// else if (isPresent(lineString, "upload_store"))
	// {
	// 	this->uploadStore = getStringDirective(lineString, "upload_store");
	// 	this->uploadStoreSet = true;
	// }
}

std::vector<ServerBlock> ConfigBlock::getVirtualHostVector(void) const
{
	return this->virtualHostVector;
}

int ConfigBlock::getClientMaxBodySize(void) const
{
	return this->clientMaxBodySize;
}

bool ConfigBlock::getAutoindex(void) const
{
	return this->autoindex;
}

std::vector<std::string> ConfigBlock::getIndex(void) const
{
	return this->index;
}

std::string ConfigBlock::getRoot(void) const
{
	return this->root;
}




