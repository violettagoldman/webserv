/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:47 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/13 19:40:26 by ashishae         ###   ########.fr       */
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

/*
** Read a server context block (between curly braces) from a file into an
** internal currentBlock variable.
** @param myfile An opened input file stream with the config
** @ret int Result: 1 if the block was read correctly, 0 if no block was found
** 				and throw an exception if block wasn't closed.
*/
int Reader::get_block(std::ifstream &myfile)
{
	std::string line;
	int balance = -1;
	int pos = 0;
	while (std::getline(myfile, line))
	{
		if ((pos = line.find('{')) != std::string::npos)
		{
			while (std::getline(myfile,line))
			{
				if (line.find('}') != std::string::npos)
				{
					return 1;
				}
				this->currentBlock.push_back(line);
			}
			throw SyntaxError();
		}
	}
	return 0;
}

void print_bag(std::vector<std::string> bag)
{
	for (int i = 0; i < bag.size(); i++)
	{
		std::cout << bag[i] << std::endl;
	}
}

/*
** Read the whole file in a single
** std::string using only open and read.
** @param filename The name of the file to open
** @ret std::string The content of the whole file.
** TODO: exceptions
*/
std::string read_file(std::string filename)
{
	int fd = open(filename.c_str(), O_RDONLY);

	char buf[BUFFER_SIZE+1];
	std::string target;

	int ret;
	while ((ret = read(fd, buf, BUFFER_SIZE)))
	{
		buf[ret] = '\0';
		target.append(buf);
	}
	close(fd);
	return target;
}

/*
** Grab a block between two top-level curly braces
** from a given string.
** @param file The string containing a block
** @param start The first character to check in file
** @ret std::string The resulting block
*/
std::string get_block(std::string file, size_t start)
{
	size_t block_start = file.find("{", start);
	size_t i = block_start;
	std::stack<char> found_brackets;
	size_t block_end;

	while (i < file.size())
	{
		if (file[i] == '{')
		{
			found_brackets.push('{');
		}
		else if (file[i] == '}')
		{
			if (found_brackets.top() == '{')
			{
				found_brackets.pop();
			}
		}
		if (found_brackets.empty())
		{
			block_end = i;
			break;
		}
		i++;
	}
	return (file.substr(block_start+1, block_end-block_start-1));

}

/*
** Get server blocks from a string that represents a configuration file
** @param file The content of a configuration file
** @ret std::vector<std::string> A vector of strings with blocks
*/
std::vector<std::string> get_server_blocks(std::string file)
{
	std::vector<std::string> v;

	size_t pos = 0;
	size_t start;
	while ((start = file.find("server ", pos)) != std::string::npos)
	{
		v.push_back(get_block(file, start));
		pos = start+1;
	}
	return v;
}

enum parserMode
{
	idle = 0,
	server = 1,
	location = 2,
};

// void parse_location( )

std::string getDirective(size_t needle, std::string line)
{
	while (isspace(line[needle]))
	{
		needle++;
	}
	// TODO: exception if no ;
	return line.substr(needle, line.find(";", needle)-needle);
}

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
		std::string pattern = lineString.substr(needle+8, lineString.find(";", needle+8)-needle+8);
		trimWhitespace(pattern);
		lp.pattern = pattern;
	}
}

void Reader::parse_location()
{
	std::stack<char> foundBrackets;

	do
	{
		if (blockEnded(line, foundBrackets))
		{
			break;
		}
		parse_location_line();
	}
	while ((ret = get_next_line(fd, &line)));
	// return Config(cp.listenIp, cp.serverName, cp.locations);
	// configVector.push_back(Config(cp.listenIp, cp.serverName, cp.locations));
	cp.locations.push_back(Location(lp.pattern, "root", lp.root));
}

// Забираем информацию из строк
void Reader::parse_server_line()
{
	std::string lineString(line);
	size_t needle;

	std::cout << "server line " << lineString << std::endl;

	if ((needle = lineString.find("listen")) != std::string::npos)
	{
		cp.listenIp = std::atoi(getDirective(needle+7, lineString).c_str());
	}
	else if ((needle = lineString.find("server_name")) != std::string::npos)
	{
		cp.serverName = split(getDirective(
			needle+12, lineString), ' ');
	}
	else if ((needle = lineString.find("location")) != std::string::npos)
	{
		parse_location();
	}
}


// Отбираем строки
void Reader::parse_server()
{
	std::cout << "Parsing server" << std::endl;
	std::stack<char> foundBrackets;
	std::string ls;
	do
	{
		if (blockEnded(line, foundBrackets))
		{
			break;
		}
		ls.assign(line);
		// TODO на лок надо переходить тут
		if (ls.find("location") != std::string::npos)
			foundBrackets.pop();
		parse_server_line();
	}
	while ((ret = get_next_line(fd, &line)));
	std::cout << "Creating server: " << cp.listenIp << std::endl;
	for (int i = 0; i < cp.serverName.size(); i++)
	{
		std::cout << cp.serverName[i] << std::endl;
	}
	// return Config(cp.listenIp, cp.serverName, cp.locations);
	configVector.push_back(Config(cp.listenIp, cp.serverName, cp.locations));
}

// Находим начало контекста
void Reader::parse()
{
	std::string l;

	l.assign(line);
	// std::cout << "Parse: " << line << std::endl;
	if (l.find("server") != std::string::npos)
	{
		this->parse_server();
	}

	std::cout << line << std::endl;
}


/*
** Create a Reader object for a given file
*/
Reader::Reader(std::string filename)
{
	// std::string content =  read_file(filename);
	// std::vector<std::string> server_blocks = get_server_blocks(content);

	// for (int i = 0; i < server_blocks.size(); ++i)
	// {
	// 	configVector.push_back(Config(server_blocks[i]));
	// 	/* code */
	// }

	
	// std::ifstream myfile(filename);
	// if (myfile.is_open())
	// {
	// 	while (get_block(myfile))
	// 	{
	// 		print_bag(currentBlock);
	// 		Config config(currentBlock);
	// 		configVector.push_back(config);
	// 		currentBlock.clear();
	// 	}
	// 	myfile.close();
	// }
		

	fd = open(filename.c_str(), O_RDONLY);

	while ((ret = get_next_line(fd, &line)))
	{
		// parse_line(fd, line, configVector);
		std::cout << "Preparing to parse: " << line << std::endl;
		parse();
	}
}

std::vector<Config> Reader::getConfigVector(void) const
{
	return configVector;
}



int main(void)
{
	std::string filename = "nginx.conf";

	Reader r(filename);
	// std::cout << read_file(filename) << std::endl;
	// std::string s = read_file(filename);
	// std::vector<std::string> vs = get_server_blocks(s);
	

	
	// std::cout << line << std::endl;
	
}


// int main(void)
// {
// 	Reader r("nginx.conf");
// 	// std::vector<std::string> v = split("a b c", ' ');
// 	// for (int i = 0; i < v.size(); i++)
// 	// {
// 	// 	std::cout << v[i] << std::endl;
// 	// }
// }