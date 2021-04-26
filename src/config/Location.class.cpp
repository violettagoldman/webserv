/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.class.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/25 17:43:54 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.class.hpp"

/*
** Constructor for Location
*/
Location::Location(ConfigFile *confFile) :
	ABlock(confFile),
	limitExcept(confFile),
	uploadStore(""),
	rootSet(false),
	cgiSet(false),
	uploadStoreSet(false)
{
}

Location::Location() :
	pattern(""),
	limitExcept(),
	cgiPath(""),
	uploadStore(""),
	rootSet(false),
	cgiSet(false),
	uploadStoreSet(false),
	cgiExtension()
{
}

Location::Location(const Location &copy) :
	ABlock(copy),
	pattern(copy.pattern),
	limitExcept(copy.limitExcept),
	cgiPath(copy.cgiPath),
	uploadStore(copy.uploadStore),
	rootSet(copy.rootSet),
	cgiSet(copy.cgiSet),
	uploadStoreSet(copy.uploadStoreSet),
	cgiExtension(copy.cgiExtension)
{
}

Location &Location::operator=(const Location &operand)
{
	pattern = operand.pattern;
	limitExcept = operand.limitExcept;
	cgiPath = operand.cgiPath;
	uploadStore = operand.uploadStore;
	rootSet = operand.rootSet;
	cgiSet = operand.cgiSet;
	uploadStoreSet = operand.uploadStoreSet;
	cgiExtension = operand.cgiExtension;
	return (*this);
}

Location::~Location()
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
	cgiSet(false),
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
		this->getConfFile()->rewind();
		throw Exception("location has to have a pattern.");
	}
	return pattern;
}

/*
** Check if the Location block is well formed.
*/
void Location::check()
{
	ABlock::checkCommon();
	// if (rootSet == true && cgiSet == true)
	// 	throw Exception("Root and fcgi_pass on the same location.");
	// if (cgiSet == true && uploadStoreSet == true)
	// 	throw Exception("Upload_store and fcgi_pass on the same location.");

	if (!this->limitExcept.isEmpty() &&
			this->limitExcept.getMethods().size() == 0)
		throw Exception("limit_except must specify at least one method.");

	if (this->getRoot() == "" && this->cgiPath == "" && this->uploadStore == "")
		throw Exception("location has to specify either root, cgi_path or upload.");
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
	if (isPresent(lineString, "cgi_path"))
	{
		this->cgiPath = getStringDirective(lineString, "cgi_path");
		this->cgiSet = true;
	}
	if (isPresent(lineString, "cgi_extension"))
	{
		this->cgiExtension = getStringDirective(lineString, "cgi_extension");
	}
	else if (isPresent(lineString, "upload_store"))
	{
		this->uploadStore = getStringDirective(lineString, "upload_store");
		this->uploadStoreSet = true;
	}
}

/*
** Return true if request has the right authorization header, and false otherwise.
** @param r Request
** @ret bool If the request is authorized for this location.
*/
bool Location::authenticate(Request &r)
{
	std::vector<Header> headers = r.getHeaders();

	std::string authValue;
	for (size_t i = 0; i < headers.size(); i++)
	{
		if (headers[i].getName() == "Authorization")
		{
			if (headers[i].getValue().size() != 1)
				return false;
			authValue = headers[i].getValue()[0];
		}
	}
	
	std::vector<std::string> parts = ft_split(authValue, ' ');

	if (parts.size() != 2)
	{
		return false;
	}

	std::string user;
	std::string password;

	if (parts[0] == "Basic")
	{
		std::string decodedCreds;

		try 
		{
			decodedCreds = Base64(parts[1]).decode();
		}
		catch (Exception &e) {
			return false;
		}
		

		std::vector<std::string> creds = ft_split(decodedCreds, ':');
		if (creds.size() != 2)
		{
			return false;
		}
		user = creds[0];
		password = creds[1];
	}
	else
	{
		return false;
	}
	
	for (size_t i = 0; i < this->getCredentials().size(); i++)
	{
		if (this->getCredentials()[i].username == user &&
			this->getCredentials()[i].password == password )
			return true;
	}
	return false;
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
** Getter for cgiPath.
*/
std::string Location::getCgiPath(void) const
{
	return this->cgiPath;
}

/*
** Legacy getter for fcgiPass.
*/
std::string Location::getFcgiPass(void) const
{
	return this->cgiPath;
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
** Getter for cgiSet.
*/
bool Location::getCgiSet(void) const
{
	return this->cgiSet;
}

/*
** Getter for uploadStoreSet.
*/
bool Location::getUploadStoreSet(void) const
{
	return this->uploadStoreSet;
}

/*
** Getter for cgiExtension
*/
std::string Location::getCgiExtension(void) const
{
	return this->cgiExtension;
}

/*
** Setter for uploadStore.
*/
void Location::setUploadStore(std::string _uploadStore)
{
	this->uploadStore = _uploadStore;
}
