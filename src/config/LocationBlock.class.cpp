/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.class.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationBlock.class.hpp"

LocationBlock::LocationBlock(ConfigFile &confFile) : ABlock(confFile)
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
std::string LocationBlock::parsePattern(std::string line)
{
	size_t loc_position = line.find("location");
	size_t pattern_start = loc_position + 9;
	size_t pattern_end = line.find("{", pattern_start);
	std::string pattern = line.substr(pattern_start, pattern_end-pattern_start);
	trimWhitespace(pattern);
	return pattern;
}

void LocationBlock::inheritParams(int _clientMaxBodySize, bool _autoindex,
		std::string _root, std::vector<std::string> _index, std::string _uploadStore)
{
	this->clientMaxBodySize = _clientMaxBodySize;
	this->autoindex = _autoindex;
	this->root = _root;
	this->index = _index;
	this->uploadStore = _uploadStore;
}

void LocationBlock::check()
{
	if (rootSet == true && fcgiSet == true)
		throw Exception("Root and fcgi_pass on the same location.");
	if (fcgiSet == true && uploadStoreSet == true)
		throw Exception("Upload_store and fcgi_pass on the same location.");
}

// void LocationBlock::inheritParams(ABlock *parent)
// {
// 	this->clientMaxBodySize = parent->getClientMaxBodySize;
// }

void LocationBlock::handleLine(std::string lineString)
{
	// std::cout << "LocationBlock handled: " << lineString << std::endl;

	if (lineString.find("limit_except") != std::string::npos)
	{
		LimitExceptBlock lExcept(this->getConfFile());
		lExcept.handle();

		limitExceptVector.push_back(lExcept);
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

std::string LocationBlock::getPattern(void) const
{
	return this->pattern;
}

std::string LocationBlock::getRoot(void) const
{
	return this->root;
}

int LocationBlock::getClientMaxBodySize(void) const 
{
	return this->clientMaxBodySize;
}

bool LocationBlock::getAutoindex(void) const
{
	return this->autoindex;
}

std::vector<std::string> LocationBlock::getIndex(void) const
{
	return this->index;
}

std::vector<LimitExceptBlock> LocationBlock::getLimitExceptVector(void) const
{
	return this->limitExceptVector;
}

std::string LocationBlock::getFcgiPass(void) const
{
	return this->fcgiPass;
}

std::map<std::string, std::string> LocationBlock::getFcgiParams(void) const
{
	return this->fcgiParams;
}

std::string LocationBlock::getUploadStore(void) const
{
	return this->uploadStore;
}

bool LocationBlock::getRootSet(void) const
{
	return this->rootSet;
}

bool LocationBlock::getFcgiSet(void) const
{
	return this->fcgiSet;
}

bool LocationBlock::getUploadStoreSet(void) const
{
	return this->uploadStoreSet;
}
