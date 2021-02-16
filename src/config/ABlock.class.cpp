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

// const std::string ABlock::blockStartKeyword = "server";

// bool ABlock::blockStarted(std::string lineString)
// {
// 	size_t pos;
// 	if ((pos = lineString.find(blockStartKeyword)) != std::string::npos)
// 	{
// 		return true;
// 	}
// 	return false;
// }

ABlock::ABlock(ConfigFile &confFile) :
	_confFile(confFile)
{
}

int ABlock::countOccurence(std::string s, char c)
{
	return std::count(s.begin(), s.end(), c);
}

void ABlock::checkLine(std::string lineString)
{
	if (countOccurence(lineString, '}') > 1)
		throw Exception("Please close each block on a new line.");
	if ((lineString.find(';', 0) != std::string::npos && \
		lineString.find(';', 0) != (lineString.size() - 1)) ||
			countOccurence(lineString, ';') > 1)
		throw Exception("Please only put one directive per line.");
}

void ABlock::check(void)
{
}

ABlock::~ABlock(void)
{
}

void ABlock::handle()
{
	bool blockClosed = false;
	do
	{
		checkLine(_confFile.getLineString());

		if (_confFile.getLineString().find("}") != std::string::npos)
		{
			blockClosed = true;
			break;
		}

		handleLine(_confFile.getLineString());

	}
	while(_confFile.getNext());
	if (!blockClosed)
		throw Exception("A block wasn't closed");
	check();
}

void ABlock::handleLine(std::string lineString)
{
	std::cout << "ABlock handled: " << lineString << std::endl;
}

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

// // TODO : test some more
int	ft_atoi(const char *str)
{
	int		nbr;
	int		sign;

	nbr = 0;
	sign = 1;
	while ((*str) == '\t' || (*str) == '\n' || (*str) == '\v' || (*str) == '\f'
			|| (*str) == '\r' || (*str) == ' ')
		str++;
	if ((*str) == '-' || (*str) == '+')
	{
		sign *= ((*str) == '-' ? -1 : 1);
		str++;
	}
	while ((*str) != '\0' && (*str) >= '0' && (*str) <= '9')
	{
		nbr *= 10;
		nbr += (*str) - '0';
		str++;
	}
	return (nbr * sign);
}

/*
** Split a string s, separated by delimiter c
** @param s String that needs to be splitted
** @param c The delimiter
** @ret std::vector<std::string> the vector of resulting separated strings
*/
std::vector<std::string> ft_split(std::string s, char c)
{
	std::vector<std::string> ret;
	int i = 0;
	size_t pos;
	while ((pos = s.find(c, i)) != std::string::npos)
	{
		ret.push_back(s.substr(i, pos-i));
		i = pos+1;
	}
	ret.push_back(s.substr(i));
	return ret;
}


// int parse_size(size_t needle, std::string lineString)
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

// void Reader::parseFcgiParam(size_t needle)
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

bool ABlock::parseBoolDirective(std::string lineString, std::string key)
{
	std::string directive = getStringDirective(lineString, key);

	if (directive == "on")
		return true;
	else
		return false;
	// TODO exception
}

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