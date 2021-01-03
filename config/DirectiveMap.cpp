/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveMap.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/02 17:07:58 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/02 17:29:02 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DirectiveMap.hpp"

/*
** read_directive() creates a directive from a string containing a valid
** directive
** @param input A string containing a valid directive
** @ret directive A resulting directive object
*/
directive read_directive(std::string input)
{
	directive dir;
	int i = 0;
	while (i < input.size() && isspace(input[i]))
		i++;
	dir.title = input.substr(i, input.find_first_of(" \t\n\v\f\r", i)-i);
	i = input.find_first_of(" \t\n\v\f\r", i);
	while (i < input.size() && isspace(input[i]))
		i++;
	dir.value = input.substr(i, input.find(';')-i);
	return dir;

}

/*
** Get directives from a block.
** At this moment, we only read the directives before the first nested block
** i.e. the first location block inside a server block
** @param bag — a vector of strings representing each line in config file
*/
DirectiveMap::DirectiveMap(std::vector<std::string> block)
{
	directive dir;
	for (int i = 0; i < block.size(); i++)
	{
		if (block[i].find('{') != std::string::npos)
		{
			return;
		}
		if (block[i].size() > 2)
		{
			dir = read_directive(block[i]);
			this->map[dir.title] = dir.value;
			std::cout << "Read directive " << dir.title << " with value " << dir.value << std::endl;
		}
	}
}

dirmap DirectiveMap::getMap(void) const
{
	return this->map;
}