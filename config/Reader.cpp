/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:47 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/11 18:04:38 by ashishae         ###   ########.fr       */
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

/*
** Create a Reader object for a given file
*/
Reader::Reader(std::string filename)
{
	std::string content =  read_file(filename);
	std::vector<std::string> server_blocks = get_server_blocks(content);

	for (int i = 0; i < server_blocks.size(); ++i)
	{
		configVector.push_back(Config(server_blocks[i]));
		/* code */
	}
	
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
		
}

std::vector<Config> Reader::getConfigVector(void) const
{
	return configVector;
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