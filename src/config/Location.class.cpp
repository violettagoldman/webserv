/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.class.hpp"

Location::Location(ConfigFile &confFile) : ABlock(confFile), limitExcept(confFile)
{
	this->rootSet = false;
	this->fcgiSet = false;
	this->uploadStoreSet = false;
}

/*
** Parse a pattern from the first line of a location block.
** From this line:
**
** location /app { ... }
**
** The function will return "/app".
** @param line The line containing the pattern.
** @ret std::string The pattern.
*/
std::string Location::parsePattern(std::string line)
{
	size_t loc_position = line.find("location");
	size_t pattern_start = loc_position + 9;
	size_t pattern_end = line.find("{", pattern_start);
	std::string pattern = line.substr(pattern_start, pattern_end-pattern_start);
	trimWhitespace(pattern);
	return pattern;
}

void Location::inheritParams(int _clientMaxBodySize, bool _autoindex,
		std::string _root, std::vector<std::string> _index, std::string _uploadStore)
{
	this->clientMaxBodySize = _clientMaxBodySize;
	this->autoindex = _autoindex;
	this->root = _root;
	this->index = _index;
	this->uploadStore = _uploadStore;
}

void Location::check()
{
	if (rootSet == true && fcgiSet == true)
		throw Exception("Root and fcgi_pass on the same location.");
	if (fcgiSet == true && uploadStoreSet == true)
		throw Exception("Upload_store and fcgi_pass on the same location.");
}

// void Location::inheritParams(ABlock *parent)
// {
// 	this->clientMaxBodySize = parent->getClientMaxBodySize;
// }

void Location::handleLine(std::string lineString)
{
	// std::cout << "Location handled: " << lineString << std::endl;

	if (lineString.find("limit_except") != std::string::npos)
	{
		// LimitExcept lExcept(this->getConfFile());
		limitExcept.handle();

		// limitExceptVector.push_back(lExcept);
	}

	if (isPresent(lineString, "root"))
	{
		this->root = getStringDirective(lineString, "root");
		this->rootSet = true;
	}
	if (isPresent(lineString, "location"))
	{
		this->pattern = parsePattern(lineString);
	}
	if (isPresent(lineString, "client_max_body_size"))
	{
		this->clientMaxBodySize = parseClientMaxBodySize(lineString);
	}
	if (isPresent(lineString, "autoindex"))
	{
		this->autoindex = parseBoolDirective(lineString, "autoindex");
	}
	if (isPresent(lineString, "fastcgi_pass"))
	{
		this->fcgiPass = getStringDirective(lineString, "fastcgi_pass");
		this->fcgiSet = true;
	}
	if (isPresent(lineString, "fastcgi_param"))
	{
		parseFastCGIParam(lineString, this->fcgiParams);
	}
	else if (isPresent(lineString, "index"))
	{
		this->index = ft_split(getStringDirective(lineString, "index"), ' ');
	}
	else if (isPresent(lineString, "upload_store"))
	{
		this->uploadStore = getStringDirective(lineString, "upload_store");
		this->uploadStoreSet = true;
	}
}

std::string Location::getPattern(void) const
{
	return this->pattern;
}

std::string Location::getRoot(void) const
{
	return this->root;
}

int Location::getClientMaxBodySize(void) const 
{
	return this->clientMaxBodySize;
}

bool Location::getAutoindex(void) const
{
	return this->autoindex;
}

std::vector<std::string> Location::getIndex(void) const
{
	return this->index;
}

LimitExcept Location::getLimitExcept(void) const
{
	return this->limitExcept;
}

std::string Location::getFcgiPass(void) const
{
	return this->fcgiPass;
}

std::map<std::string, std::string> Location::getFcgiParams(void) const
{
	return this->fcgiParams;
}

std::string Location::getUploadStore(void) const
{
	return this->uploadStore;
}

bool Location::getRootSet(void) const
{
	return this->rootSet;
}

bool Location::getFcgiSet(void) const
{
	return this->fcgiSet;
}

bool Location::getUploadStoreSet(void) const
{
	return this->uploadStoreSet;
}
