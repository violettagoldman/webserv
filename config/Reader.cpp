/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:47 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/03 15:23:10 by ashishae         ###   ########.fr       */
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



Reader::Reader(std::string filename)
{
	std::ifstream myfile(filename);
	if (myfile.is_open())
	{
		while (get_block(myfile))
		{
			print_bag(currentBlock);
			Config config(currentBlock);
			configVector.push_back(config);
			currentBlock.clear();
		}
		myfile.close();
	}
		
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