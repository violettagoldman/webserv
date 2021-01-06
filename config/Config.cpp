/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:43 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/06 19:48:57 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

// Config::Config()
// {
// 	listenIp = 80;
// 	serverName.push_back("localhost");
// 	Location default_location;
// 	locations.push_back(default_location);
// }

/*
** This exception should be thrown where a config omits a required
** directive, for example, serverName or listen.
** Its functionality may be extended to specify the context and the 
** directive that is missing, as well as the line number.
** @ret Exception description.
*/
const char* Config::DirectiveNotFound::what() const throw()
{
	return "A required directive wasn't found in a context.";
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
** Get a directive from a directive map or throw
** Config::DirectiveNotFound, if a directive is missing from the directives
** map.
** Intended to be used for required directives like serverName or listen.
** @param key The key to be accessed.
** @ret std::string The value at key, if present.
*/
std::string Config::read_or_throw(std::string key)
{
	std::string ret;
	ret = directives[key];
	if (ret.empty())
		throw Config::DirectiveNotFound();
	return ret;
}

void Config::readDirectives(std::vector<std::string> bag)
{
	DirectiveMap dm(bag);
	this->directives = dm.getMap();
}

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
** Read all the blocks inside a block given in input.
** As of now, only reads one-level nested blocks.
** @param input A server context block
** @ret std::vector<locationBlock> a vector of locations
*/
std::vector<locationBlock> read_child_blocks(std::vector<std::string> input)
{
	std::vector<locationBlock> ret;
	locationBlock newLocation;

	std::cout << "Reading child blocks" << std::endl;

	for (int i = 0; i < input.size(); i++)
	{
		if (input[i].find('{') != std::string::npos)
		{
			while (i < input.size())
			{
				if (input[i].find("location") != std::string::npos)
				{
					// Grab pattern;
					size_t pattern_begin = input[i].find("location") + 9;
					size_t pattern_end = input[i].find('{', pattern_begin);
					newLocation.pattern = input[i].substr(pattern_begin, pattern_end - pattern_begin);
					trimWhitespace(newLocation.pattern);
					
					// For now, whatever is left in the block is location.
					for (int j = i+1; j < input.size(); j++)
					{
						newLocation.block.push_back(input[j]);
					}
					ret.push_back(newLocation);
					
				}
				i++;


				
				// if (input[i].find('}') != std::string::npos)
				// {
				// 	ret.push_back(newLocation);
				// 	newLocation.block.clear();
				// 	newLocation.pattern = "";
				// 	break;
				// }
				// newLocation.block.push_back(input[i]);
			}
		}
	}
	return ret;
}

void Config::readLocations(std::vector<std::string> block) {
	std::vector<locationBlock> locationBlocks = read_child_blocks(block);
	locationBlock lb;
	for (int i = 0; i < locationBlocks.size(); i++)
	{
		lb = locationBlocks[i];
		this->locations.push_back(Location(lb.pattern, lb.block));
	}
}

/*
** Read a Config from a vector of strings, representing a single server block
** @param bag A vector of strings, where each string is a line from a single
** 				server context.
*/
Config::Config(std::vector<std::string> block)
{
	readDirectives(block);
	readLocations(block);
	std::cout << "Read config" << std::endl;
	
	// TODO: redo atoi....
	this->listenIp = atoi(read_or_throw("listen").c_str());
	this->serverName = split(read_or_throw("server_name"), ' ');
}

// Config::Config(int _listenIp, std::vector<std::string> _serverName,
// 				std::vector<Location> _locations)
// {
// 	listenIp = _listenIp;
// 	serverName = _serverName;
// 	locations = _locations;
// }

Config::~Config()
{
}

int Config::getListenIp(void) const
{
	return this->listenIp;
}

std::vector<std::string> Config::getServerName(void) const
{
	return this->serverName;
}

std::vector<Location> Config::getLocations(void) const
{
	return this->locations;
}


// Config::Config(const Config &copy)
// {
	
// }

// Config::Config &operator= (const Config &operand)
// {
	
// }

