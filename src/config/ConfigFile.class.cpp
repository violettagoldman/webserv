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

ConfigFile::ConfigFile() : _fd(-1)
{
}

void ConfigFile::openFile(std::string filename)
{
	this->_fd = open(filename.c_str(), O_RDONLY);
	if (_fd < 0)
		throw Exception("Couldn't open file");
}

ConfigFile::ConfigFile(std::string filename) :
	_lastLineRead(false), _lastLineSent(false)
{
	this->_fd = open(filename.c_str(), O_RDONLY);
	if (_fd < 0)
		throw Exception("Couldn't open file");
}

void pass_through_select(int fd)
{
	fd_set rfds;

	FD_ZERO(&rfds);

	FD_SET(fd, &rfds);

	struct timeval tv;

	/* Wait up to five seconds. */

	tv.tv_sec = 5;
	tv.tv_usec = 0;

	int retval = select(fd + 1, &rfds, NULL, NULL, &tv);

	if (retval == -1)
		throw Exception("Error while trying to select().");
	else if (retval)
		return ;
	else
		throw Exception("Timeout while trying to read from config file.");
}

int ConfigFile::getNext()
{
	if (this->_fd < 0)
		throw Exception("File not open");

	if (_lastLineSent)
		return 0;

	if (_lastLineRead)
	{
		_lastLineSent = true;
		return 0;
	}

	pass_through_select(this->_fd);

	this->_ret = fd_get_next_line(this->_fd, &(this->_line));
	this->_lineString.assign(this->_line);
	if (_ret == 0)
	{
		_lastLineRead = true;
		close(_fd);
		return 1;
	}
	return this->_ret;
}

std::string ConfigFile::getLineString(void) const
{
	if (this->_fd < 0)
		throw Exception("File not open");
	return this->_lineString;
}

void ConfigFile::rewind(void)
{
	while (this->getNext())
		continue;
}