/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 16:35:08 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/13 14:37:23 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <stack>
#include "get_next_line/get_next_line.h"


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