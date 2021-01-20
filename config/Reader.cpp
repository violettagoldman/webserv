/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:47 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/20 19:28:31 by ashishae         ###   ########.fr       */
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

bool parseBoolDirective(std::string directive)
{
	if (directive == "on")
		return true;
	else
		return false;
	// TODO exception

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
		lp.clientMaxBodySize = std::atoi(
			getDirective(needle+20, lineString).c_str());
	}
	if ((needle = lineString.find("autoindex")) != std::string::npos)
	{
		lp.autoindex = parseBoolDirective(getDirective(needle+9, lineString));
	}
}

void Reader::resetLocationPrototype()
{
	lp.pattern = "";
	lp.root = "";
	lp.clientMaxBodySize = vhp.clientMaxBodySize;
	lp.autoindex = vhp.autoindex;
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
		vhp.listenIp = std::atoi(
			lineString.substr(hostEnd+1, valueEnd - hostEnd).c_str());
	}
	else
	{
		vhp.listenHost = "";
		vhp.listenIp = std::atoi(getDirective(valueStart, lineString).c_str());
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
		vhp.clientMaxBodySize = std::atoi(
			getDirective(needle+20, lineString).c_str());
	}
	else if ((needle = lineString.find("autoindex")) != std::string::npos)
	{
		vhp.autoindex = parseBoolDirective(getDirective(needle+9, lineString));
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
	vhp.clientMaxBodySize = 0;
	vhp.autoindex = false;
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
** Parse the file, reading server blocks into virtualHost objects in virtualHostVector
** member.
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
		cp.clientMaxBodySize = std::atoi(
			getDirective(needle+20, lineString).c_str());
	}
	else if ((needle = lineString.find("autoindex")) != std::string::npos)
	{
		vhp.autoindex = parseBoolDirective(getDirective(needle+9, lineString));
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