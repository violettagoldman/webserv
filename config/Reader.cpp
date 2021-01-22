/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:47 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/22 12:33:38 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reader.hpp"

/*
** Erases all the whitespace characters in the end of a string
** @param s The input string
*/
void trimWhitespace(std::string &s)
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
void trimWhitespaceStart(std::string &s)
{
	std::string::iterator it = s.begin();
	while (isspace(*it))
	{
		s.erase(it);
	}
}

/*
** Split a string s, separated by delimiter c
** @param s String that needs to be splitted
** @param c The delimiter
** @ret std::vector<std::string> the vector of resulting separated strings
*/
std::vector<std::string> split(std::string s, char c)
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
** Get the value of a directive from the line.
** From this string:
**
** server_name			alpha bravo charlie;
**
** It will return "alpha bravo charlie".
** @param needle The position of the directive (the index of the letter s
**		in the example).
** @param line The line containing the directive.
** @ret std::string The string containing the value of the directive.
*/
std::string getDirective(size_t needle, std::string line)
{
	while (isspace(line[needle]))
	{
		needle++;
	}
	// TODO: exception if no ;
	return line.substr(needle, line.find(";", needle)-needle);
}

// TODO: check overflow
int parse_size(size_t needle, std::string lineString)
{
	std::string value = getDirective(needle, lineString);
	int raw_value = ft_atoi(value.c_str());
	size_t semicolon = lineString.find(";", needle);
	if (lineString[semicolon-1] == 'm' || lineString[semicolon-1] == 'M')
	{
		raw_value *= 1000000;
	}
	else if (lineString[semicolon-1] == 'k' || lineString[semicolon-1] == 'k')
	{
		raw_value *= 1000;
	}
	return raw_value;
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
std::string parse_pattern(std::string line)
{
	size_t loc_position = line.find("location");
	size_t pattern_start = loc_position + 9;
	size_t pattern_end = line.find("{", pattern_start);
	std::string pattern = line.substr(pattern_start, pattern_end-pattern_start);
	trimWhitespace(pattern);
	return pattern;
}

std::string parse_method(std::string line)
{
	size_t loc_position = line.find("limit_except");
	size_t pattern_start = loc_position + 13;
	size_t pattern_end = line.find("{", pattern_start);
	std::string pattern = line.substr(pattern_start, pattern_end-pattern_start);
	trimWhitespace(pattern);
	return pattern;
}


bool parseBoolDirective(std::string directive)
{
	if (directive == "on")
		return true;
	else
		return false;
	// TODO exception

}

void Reader::resetLimitExceptPrototype()
{
	lep.allow.clear();
	lep.deny.clear();
	lep.method = "";
}

void Reader::parse_limit_except_line()
{
	size_t needle;
	std::vector<std::string> appendix;

	if ((needle = lineString.find("limit_except")) != std::string::npos)
	{
		lep.method = parse_method(lineString);
	}
	if ((needle = lineString.find("allow")) != std::string::npos)
	{
		appendix = split(getDirective(needle+5, lineString), ' ');
		lep.allow.insert(lep.allow.end(), appendix.begin(), appendix.end());
	}
	if ((needle = lineString.find("deny")) != std::string::npos)
	{
		appendix = split(getDirective(needle+4, lineString), ' ');
		lep.deny.insert(lep.deny.end(), appendix.begin(), appendix.end());
	}
}


void Reader::parse_limit_except()
{
	resetLimitExceptPrototype();
	do
	{
		lineString.assign(line);
		if (lineString.find("}") != std::string::npos)
		{
			break;
		}
		parse_limit_except_line();
	}
	while ((ret = get_next_line(fd, &line)));
	lp.limitExcept = LimitExcept(lep);
}


// TODO: https://github.com/nginx/nginx/blob/master/conf/fastcgi_params
void Reader::parseFcgiParam(size_t needle)
{
	while(isspace(lineString[needle]))
	{
		needle++;
	}
	size_t keyStart = needle;
	while(!isspace(lineString[needle]))
	{
		needle++;
	}
	size_t keyEnd = needle;
	while(isspace(lineString[needle]))
	{
		needle++;
	}
	size_t valueStart = needle;
	size_t valueEnd = lineString.find(";", valueStart);
	// TODO: exception
	std::string key = lineString.substr(keyStart, keyEnd-keyStart);
	std::string value = lineString.substr(valueStart, valueEnd-valueStart);
	lp.fcgiParams[key] = value;
}

/*
** Parse a single location line, filling the locationPrototype member.
*/
void Reader::parse_location_line()
{
	size_t needle;

	if ((needle = lineString.find("root")) != std::string::npos)
	{
		lp.root = getDirective(needle+4, lineString);
	}
	if ((needle = lineString.find("location")) != std::string::npos)
	{
		lp.pattern = parse_pattern(lineString);
	}
	if ((needle = lineString.find(
				"client_max_body_size")) != std::string::npos)
	{
		lp.clientMaxBodySize = parse_size(needle+20, lineString);
	}
	if ((needle = lineString.find("autoindex")) != std::string::npos)
	{
		lp.autoindex = parseBoolDirective(getDirective(needle+9, lineString));
	}
	if ((needle = lineString.find("fastcgi_pass")) != std::string::npos)
	{
		lp.fcgiPass = getDirective(needle+12, lineString);
	}
	if ((needle = lineString.find("fastcgi_param")) != std::string::npos)
	{
		parseFcgiParam(needle+13);
	}
	else if ((needle = lineString.find("index")) != std::string::npos)
	{
		lp.index = split(getDirective(
			needle+5, lineString), ' ');
	}
}

void Reader::resetLocationPrototype()
{
	lp.pattern = "";
	lp.root = "";
	lp.clientMaxBodySize = vhp.clientMaxBodySize;
	lp.autoindex = vhp.autoindex;
	lp.limitExcept = LimitExcept();
	lp.fcgiPass = "";
	lp.fcgiParams.clear();
}

/*
** Get next lines until the end of the location context, and parse
** the location context into locationPrototype. Once the block ends,
** create Location and add it to the virtualHostPrototype's locations vector.
*/
void Reader::parse_location()
{
	resetLocationPrototype();
	do
	{
		lineString.assign(line);
		
		if (lineString.find("}") != std::string::npos)
		{
			break;
		}
		if (lineString.find("limit_except") != std::string::npos)
			parse_limit_except();
		else
			parse_location_line();
	}
	while ((ret = get_next_line(fd, &line)));
	vhp.locations.push_back(Location(lp));
}

// TODO: rewrite atoi
/*
** Parse the listen directive into the virtualHostPrototype member.
** @param needle The position where the directive starts (the index of 'l').
*/
void Reader::parse_listen(size_t needle)
{
	size_t valueStart = needle + 7;
	size_t hostEnd;
	if ((hostEnd = lineString.find(":")) != std::string::npos)
	{
		vhp.listenHost = lineString.substr(valueStart, hostEnd - valueStart);
		trimWhitespaceStart(vhp.listenHost);
		size_t valueEnd = lineString.find(";", hostEnd);
		vhp.listenIp = ft_atoi(
			lineString.substr(hostEnd+1, valueEnd - hostEnd).c_str());
	}
	else
	{
		vhp.listenHost = "";
		vhp.listenIp = ft_atoi(getDirective(valueStart, lineString).c_str());
	}
}

/*
** Parse a single line in the server context, filling the virtualHostPrototype
** member.
*/
void Reader::parse_server_line()
{
	std::string lineString(line);
	size_t needle;

	if ((needle = lineString.find("listen")) != std::string::npos)
	{
		parse_listen(needle);
	}
	else if ((needle = lineString.find("server_name")) != std::string::npos)
	{
		vhp.serverName = split(getDirective(
			needle+12, lineString), ' ');
	}
	else if ((needle = lineString.find(
				"client_max_body_size")) != std::string::npos)
	{
		vhp.clientMaxBodySize = parse_size(needle+20, lineString);
	}
	else if ((needle = lineString.find("autoindex")) != std::string::npos)
	{
		vhp.autoindex = parseBoolDirective(getDirective(needle+9, lineString));
	}
	else if ((needle = lineString.find("index")) != std::string::npos)
	{
		vhp.index = split(getDirective(
			needle+5, lineString), ' ');
	}
}

/*
** Reset the virtualHostPrototype member into its default state.
*/
void Reader::resetVirtualHostPrototype()
{
	vhp.listenIp = -1;
	vhp.listenHost = "";
	vhp.locations.clear();
	vhp.serverName.clear();
	vhp.clientMaxBodySize = cp.clientMaxBodySize;
	vhp.autoindex = cp.autoindex;
}

/*
** Parse a server context, passing lines to parse_server_line until
** the context ends, then create the virtualHost object and add it to
** virtualHostVector member.
*/
void Reader::parse_server()
{
	resetVirtualHostPrototype();

	do
	{
		lineString.assign(line);
		if (lineString.find("}") != std::string::npos)
		{
			break;
		}
		if (lineString.find("location") != std::string::npos)
			parse_location();
		else
			parse_server_line();
	}
	while ((ret = get_next_line(fd, &line)));
	// TODO: check vhp
	cp.virtualHostVector.push_back(VirtualHost(vhp));
}

/*
** Parse the file, reading server blocks into virtualHost objects in
** virtualHostVector member.
*/
void Reader::parse()
{
	size_t needle;
	if (lineString.find("server") != std::string::npos)
	{
		this->parse_server();
	}
	else if ((needle = lineString.find(
				"client_max_body_size")) != std::string::npos)
	{
		cp.clientMaxBodySize = parse_size(needle+20, lineString);
	}
	else if ((needle = lineString.find("autoindex")) != std::string::npos)
	{
		cp.autoindex = parseBoolDirective(getDirective(needle+9, lineString));
	}
	else if ((needle = lineString.find("index")) != std::string::npos)
	{
		cp.index = split(getDirective(
			needle+5, lineString), ' ');
	}
}


/*
** Create a Reader object for a given file.
** @param filename Filename of the file to parse.
*/
Reader::Reader(std::string filename)
{
	fd = open(filename.c_str(), O_RDONLY);
	//TODO: check file errors

	while ((ret = get_next_line(fd, &line)))
	{
		lineString.assign(line);
		parse();
	}
}

Config *Reader::createConfig()
{
	return new Config(cp);
}