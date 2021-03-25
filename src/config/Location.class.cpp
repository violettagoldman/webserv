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

/*
** Constructor for Location
*/
Location::Location(ConfigFile &confFile) :
	ABlock(confFile),
	limitExcept(confFile),
	uploadStore(""),
	rootSet(false),
	fcgiSet(false),
	uploadStoreSet(false)
{
}

/*
** Constructor for Location that inherits the parent block directives.
*/
Location::Location(ABlock &ab) :
	ABlock(ab),
	limitExcept(ab.getConfFile()),
	uploadStore(""),
	rootSet(false),
	fcgiSet(false),
	uploadStoreSet(false)
{
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
	if (pattern.size() == 0)
	{
		this->getConfFile().rewind();
		throw Exception("location has to have a pattern.");
	}
	return pattern;
}

/*
** Check if the Location block is well formed.
*/
void Location::check()
{
	if (rootSet == true && fcgiSet == true)
		throw Exception("Root and fcgi_pass on the same location.");
	if (fcgiSet == true && uploadStoreSet == true)
		throw Exception("Upload_store and fcgi_pass on the same location.");

	if (!this->limitExcept.isEmpty() &&
			this->limitExcept.getMethods().size() == 0)
		throw Exception("limit_except must specify at least one method.");

	if (this->getRoot() == "" && this->fcgiPass == "" && this->uploadStore == "")
		throw Exception("location has to specify either root, fastcgi_pass or upload.");
}

/*
** Handle a line that belongs to the location block.
** @param lineString The line that belongs to the location block.
*/
void Location::handleLine(std::string lineString)
{
	if (lineString.find("limit_except") != std::string::npos)
	{
		limitExcept.handle();
	}
	if (isPresent(lineString, "root"))
	{
		this->rootSet = true;
	}
	if (isPresent(lineString, "location"))
	{
		this->pattern = parsePattern(lineString);
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
	else if (isPresent(lineString, "upload_store"))
	{
		this->uploadStore = getStringDirective(lineString, "upload_store");
		this->uploadStoreSet = true;
	}
}

/*
** Getter for pattern.
*/
std::string Location::getPattern(void) const
{
	return this->pattern;
}

/*
** Getter for limitExcept.
*/
LimitExcept Location::getLimitExcept(void) const
{
	return this->limitExcept;
}

/*
** Getter for fcgiPass.
*/
std::string Location::getFcgiPass(void) const
{
	return this->fcgiPass;
}

/*
** Getter for fcgiParams.
*/
std::map<std::string, std::string> Location::getFcgiParams(void) const
{
	return this->fcgiParams;
}

/*
** Getter for uploadStore.
*/
std::string Location::getUploadStore(void) const
{
	return this->uploadStore;
}

/*
** Getter for rootSet.
*/
bool Location::getRootSet(void) const
{
	return this->rootSet;
}

/*
** Getter for fcgiSet.
*/
bool Location::getFcgiSet(void) const
{
	return this->fcgiSet;
}

/*
** Getter for uploadStoreSet.
*/
bool Location::getUploadStoreSet(void) const
{
	return this->uploadStoreSet;
}

/*
** Setter for uploadStore.
*/
void Location::setUploadStore(std::string _uploadStore)
{
	this->uploadStore = _uploadStore;
}
