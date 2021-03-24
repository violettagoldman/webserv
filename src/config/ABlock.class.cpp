/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABlock.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ABlock.class.hpp"

/*
** The go-to constructor of a block. Includes the nginx defaults for
** the directives.
** @param confFile a reference to an opened ConfigFile.
*/
ABlock::ABlock(ConfigFile &confFile) :
	_confFile(confFile),
	_clientMaxBodySize(1000000),
	_autoindex(false),
	_index(std::vector<std::string>(1, "index.html")),
	_root("html")
{
}

/*
** Return the number of occurences of char c in string s
** @param s The string to look in.
** @param c The character to look for.
** @ret int The number of ocurrences
*/
int ABlock::countOccurence(std::string s, char c)
{
	return std::count(s.begin(), s.end(), c);
}

/*
** Check the lineString for all possible syntax errors and throw appropriate
** Exceptions.
*/
void ABlock::checkLine(std::string lineString)
{
	if (countOccurence(lineString, '}') > 1)
		throw Exception("Please close each block on a new line.");
	if ((lineString.find(';', 0) != std::string::npos && \
		lineString.find(';', 0) != (lineString.size() - 1)) ||
			countOccurence(lineString, ';') > 1)
		throw Exception("Please only put one directive per line.");
}

/*
** This is the function that you can overload to do some checks on the state
** of your block (like if all parameters have been set to reasonable values).
** It will be called after the block has been read completely.
*/
void ABlock::check(void)
{
}

/*
** The destructor for ABlock.
*/
ABlock::~ABlock(void)
{
}

/*
** The function that reads the whole block line by line and returns only
** when it's completely read.
*/
void ABlock::handle()
{
	bool blockClosed = false;
	do
	{
		std::string ls = _confFile.getLineString();
		checkLine(ls);

		if (ls.find("}") != std::string::npos)
		{
			blockClosed = true;
			break;
		}

		handleLineCommon(ls);
		handleLine(ls);

	}
	while(_confFile.getNext());
	if (!blockClosed)
		throw Exception("A block wasn't closed");
}

/*
** Parse the error_pages directve.
** @param directiveValue The value of that directive (whatever is between
** its name and ;, without leading whitespace)
*/
void ABlock::parseErrorPage(std::string directiveValue)
{
	std::map<int, std::string> ret;

	std::vector<std::string> directiveParts = ft_split(directiveValue, ' ');

	if (directiveParts.size() < 2)
	{
		_confFile.rewind();
		throw Exception("error_page has to specify error code and page.");
	}

	for (size_t i = 0; i < directiveParts.size()-1; i++)
	{
		ret[ft_atoi(directiveParts[i].c_str())] = directiveParts[directiveParts.size()-1];
	}
	insertErrorPages(ret);
}

/*
** Insert new error pages to the values that we may have inherited.
** std::map::insert doesn't overwrite existing elements. So in order to merge
** the two maps, we first insert old map into the new map, and then swap them.
** @param newErrorPages The new parsed error page values.
*/
void ABlock::insertErrorPages(std::map<int, std::string> &newErrorPages)
{
	newErrorPages.insert(_errorPage.begin(), _errorPage.end());
	std::swap(_errorPage, newErrorPages);
}

/*
** Handle all common directives (that is to say, those that exist on all levels
** i.e. root and index).
*/
void ABlock::handleLineCommon(std::string lineString)
{
	if (isPresent(lineString, "root"))
	{
		this->_root = getStringDirective(lineString, "root");
	}
	else if (isPresent(lineString, "client_max_body_size"))
	{
		this->_clientMaxBodySize = parseClientMaxBodySize(lineString);
	}
	else if (isPresent(lineString, "autoindex"))
	{
		this->_autoindex = parseBoolDirective(lineString, "autoindex");
	}
	else if (isPresent(lineString, "index"))
	{
		this->_index = ft_split(getStringDirective(lineString, "index"), ' ');
	}
	else if (isPresent(lineString, "error_page"))
	{
		parseErrorPage(getStringDirective(lineString, "error_page"));
	}
}

/*
** Do specific manipulation on a line (you can override this function
** to add new directives)
** @param lineString The string we received from the config file, belonging
** to this block.
*/
void ABlock::handleLine(std::string lineString)
{
	(void)lineString;
}

/*
** Return true if keyword exists in lineString, convenient to check if there's
** a certain directive in the line.
** @param lineString A line belonging to this block.
** @param keyWord A string to look for.
*/
bool ABlock::isPresent(std::string lineString, std::string keyword)
{
	return (lineString.find(keyword) != std::string::npos);
}

/*
** Get the value of a directive from the line.
** From this string:
**
** server_name			alpha bravo charlie;
**
** It will return "alpha bravo charlie".
** @param line The line containing the directive.
** @param key The key of the directive ("server_name")
** @ret std::string The string containing the value of the directive.
*/
std::string ABlock::getStringDirective(std::string lineString, std::string key)
{
	size_t keyStart = lineString.find(key);
	size_t valueStart = keyStart + key.size();

	while (isspace(lineString[valueStart]))
		valueStart++;

	size_t semicolon = lineString.find(";", keyStart);
	if (semicolon == std::string::npos)
		throw Exception("A semicolon is missing");

	return lineString.substr(valueStart, semicolon-valueStart);
}

/*
** Getter for _confFile.
*/
ConfigFile &ABlock::getConfFile(void) const
{
	return this->_confFile;
}

/*
** Erases all the whitespace characters in the end of a string
** @param s The input string
*/
void ABlock::trimWhitespace(std::string &s)
{
	std::string::iterator it = s.end();
	it--;
	while (isspace(*it))
	{
		s.erase(it);
		it--;
	}
}

/*
** Erases all the whitespace characters in the beginning of a string
** @param s The input string
*/
void ABlock::trimWhitespaceStart(std::string &s)
{
	std::string::iterator it = s.begin();
	while (isspace(*it))
	{
		s.erase(it);
	}
}

/*
** Parse the clientMaxBodySize directive.
** @param lineString The line containing the directive.
*/
int ABlock::parseClientMaxBodySize(std::string lineString)
{
	std::string value = getStringDirective(lineString, "client_max_body_size");
	int raw_value = ft_atoi(value.c_str());
	size_t semicolon = lineString.find(";", 0);
	if (lineString[semicolon-1] == 'm' || lineString[semicolon-1] == 'M')
	{
		if (raw_value > 2147)
			throw Exception("clientMaxBodySize too large");
		raw_value *= 1000000;
	}
	else if (lineString[semicolon-1] == 'k' || lineString[semicolon-1] == 'k')
	{
		if (raw_value > 2147483)
			throw Exception("clientMaxBodySize too large");
		raw_value *= 1000;
	}
	return raw_value;
}

/*
** Parse the fastcgi_param directive.
** @param lineString The line containing the directive.
** @param params The reference to the parameter map.
*/
void ABlock::parseFastCGIParam(std::string lineString,
			std::map<std::string, std::string> &params)
{
	std::string directive = getStringDirective(lineString, "fastcgi_param");
	size_t cursor = 0;
	size_t keyStart = 0;
	while(!isspace(directive[cursor]))
	{
		cursor++;
	}
	size_t keyEnd = cursor;
	while(isspace(directive[cursor]))
	{
		cursor++;
	}
	size_t valueStart = cursor;
	size_t valueEnd = directive.find(";", valueStart);
	std::string key = directive.substr(keyStart, keyEnd-keyStart);
	std::string value = directive.substr(valueStart, valueEnd-valueStart);
	params[key] = value;
}

/*
** Parse a on/off directive (ex. 'autoindex on' will return true).
** @param lineString the line containing the directive.
** @param key The name of the directive (ex. 'autoindex').
*/
bool ABlock::parseBoolDirective(std::string lineString, std::string key)
{
	std::string directive = getStringDirective(lineString, key);

	if (directive == "on")
		return true;
	else
		return false;
	// TODO exception
}

/*
** Parse a listen directive.
** @param lineString The line belonging to this block
** @param listenHost The reference to the listenHost variable that will be set
** to the actual listen host.
** @param listenIp The reference to the listenIp variable, that will be set to
** the port.
*/
void ABlock::parseListen(std::string lineString, std::string &listenHost,
							int &listenIp)
{
	std::string directive = getStringDirective(lineString, "listen");
	size_t valueStart = 0;
	size_t hostEnd;
	if ((hostEnd = directive.find(":")) != std::string::npos)
	{
		listenHost = directive.substr(valueStart, hostEnd - valueStart);
		trimWhitespaceStart(listenHost);
		size_t valueEnd = directive.find(";", hostEnd);
		listenIp = ft_atoi(
			directive.substr(hostEnd+1, valueEnd - hostEnd).c_str());
	}
	else
	{
		listenHost = "";
		listenIp = ft_atoi(directive.c_str());
	}
}

/*
** Getter for _clientMaxBodySize.
*/
int ABlock::getClientMaxBodySize(void) const
{
	return _clientMaxBodySize;
}

/*
** Getter for _autoindex.
*/
bool ABlock::getAutoindex(void) const
{
	return _autoindex;
}

/*
** Getter for _index.
*/
std::vector<std::string> ABlock::getIndex(void) const
{
	return _index;

}

/*
** Getter for _root.
*/
std::string ABlock::getRoot(void) const
{
	return _root;
}

/*
** Getter for _errorPage.
*/
std::map<int, std::string> ABlock::getErrorPage(void) const
{
	return _errorPage;
}