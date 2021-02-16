/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ConfigFile.class.hpp"

ConfigFile::ConfigFile(std::string filename) :
	lastLineRead(false), lastLineSent(false)
{
	this->fd = open(filename.c_str(), O_RDONLY);
	if (fd < 0)
		throw Exception("Couldn't open file");

	// while ((ret = fd_get_next_line(fd, &line)))
	// {
	// 	// std::cout << "Line read: " << line << std::endl;
	// 	// assignLineString();
	// 	this->lineString.assign(line);
	// 	parse(this->lineString);
	// }
	// parse(this->lineString);
}

// void ConfigFile::next()
// {
// 	this->ret = fd_get_next_line(this->fd, &(this->line));
// 	this->lineString.assign(this->line);
// }

int ConfigFile::getNext()
{
	if (lastLineSent)
		return 0;

	if (lastLineRead)
	{
		lastLineSent = true;
		return 0;
	}
	this->ret = fd_get_next_line(this->fd, &(this->line));
	this->lineString.assign(this->line);
	if (ret == 0)
	{
		lastLineRead = true;
		return 1;
	}
	return this->ret;
}

std::string ConfigFile::getLineString(void) const
{
	return this->lineString;
}