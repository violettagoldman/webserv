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