/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:47 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/16 11:34:05 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reader.hpp"

/*
** This exception should be thrown where a config omits a required
** directive, for example, serverName or listen.
** Its functionality may be extended to specify the context and the 
** directive that is missing, as well as the line number.
** @ret Exception description.
*/
const char* Reader::SyntaxError::what() const throw()
{
	return "The config file contains a syntax error.";
}


// ** This exception should be thrown where a config omits a required
// ** directive, for example, serverName or listen.
// ** Its functionality may be extended to specify the context and the 
// ** directive that is missing, as well as the line number.
// ** @ret Exception description.

// const char* Config::DirectiveNotFound::what() const throw()
// {
// 	return "A required directive wasn't found in a context.";
// }


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

// Legacy
// Находим конец 
// TODO: вернуть указатель на конец блока, чтобы можно было что-нибудь дописать
// после конца контекста
bool blockEnded(char *line, std::stack<char> &foundBrackets)
{
	std::string l(line);
	for (int i = 0; i < l.size(); i++)
	{
		if (l[i] == '{')
		{
			foundBrackets.push('{');
		}
		else if (l[i] == '}')
		{
			if (foundBrackets.top() == '{')
			{
				foundBrackets.pop();
				if (foundBrackets.empty())
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*
** Parse a pattern from a line that starts a location.
** From this line:
**
** location /app { ... }
**
** It will return "/app".
** @param line The line containing the pattern
** @ret std::string The pattern
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

/*
** Read the pattern and the directives from the location block into the
** locationPrototype.
*/
void Reader::parse_location_line()
{
	std::string lineString(line);
	size_t needle;

	if ((needle = lineString.find("root")) != std::string::npos)
	{
		lp.root = getDirective(needle+4, lineString);
	}
	else if ((needle = lineString.find("location")) != std::string::npos)
	{
		lp.pattern = parse_pattern(lineString);
	}
}

/*
** Get next lines until the end of the location context, and parse
** the location context into the locationPrototype. Once the block ends,
** create Location and add it to the configPrototype's locations vector.
*/
void Reader::parse_location()
{
	std::string lineString;
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
	cp.locations.push_back(Location(lp.pattern, "root", lp.root));
}

// Забираем информацию из строк
void Reader::parse_server_line()
{
	std::string lineString(line);
	size_t needle;

	if ((needle = lineString.find("listen")) != std::string::npos)
	{
		cp.listenIp = std::atoi(getDirective(needle+7, lineString).c_str());
	}
	else if ((needle = lineString.find("server_name")) != std::string::npos)
	{
		cp.serverName = split(getDirective(
			needle+12, lineString), ' ');
	}
}


/*
** Parse a server context.
** This function passes all the lines that are inside the server context
** to the parse_server_line function, and checks if the block has ended.
** If the block has ended, it adds the parsed server block to the 
** configVector and returns.
*/
void Reader::parse_server()
{
	std::stack<char> foundBrackets;
	std::string ls;
	if (!cp.locations.empty())
		cp.locations.clear();
	do
	{
		ls.assign(line);
		if (ls.find("}") != std::string::npos)
		{
			break;
		}
		if (ls.find("location") != std::string::npos)
			parse_location();
		else
			parse_server_line();
	}
	while ((ret = get_next_line(fd, &line)));
	// TODO: check cp
	configVector.push_back(Config(cp.listenIp, cp.serverName, cp.locations));
}

/*
** Parse the config.
** This function tries to find a server context. If it succeeds, it passes
** execution to the parse_server function.
*/
void Reader::parse()
{
	std::string l;

	l.assign(line);
	if (l.find("server") != std::string::npos)
	{
		this->parse_server();
	}
}


/*
** Create a Reader object for a given file.
** @param filename The filename of the file has to be parsed
*/
Reader::Reader(std::string filename)
{
	fd = open(filename.c_str(), O_RDONLY);
	//TODO: check file errors

	while ((ret = get_next_line(fd, &line)))
	{
		parse();
	}
}


/*
** A getter for the configVector attribute
** @ret std::vector<Config> The vector of parsed Configs
*/
std::vector<Config> Reader::getConfigVector(void) const
{
	return configVector;
}
