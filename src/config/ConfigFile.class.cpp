/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/25 17:30:45 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ConfigFile.class.hpp"

/*
** Default constructor for ConfigFile.
*/
ConfigFile::ConfigFile() : _fd(-1)
{
}

ConfigFile::ConfigFile(const ConfigFile &copy) :
	_fd(copy._fd),
	_ret(copy._ret),
	_line(copy._line),
	_lastLineRead(copy._lastLineRead),
	_lastLineSent(copy._lastLineSent),
	_lineString(copy._lineString)
{
}

ConfigFile &ConfigFile::operator=(const ConfigFile &operand)
{
	_fd = operand._fd;
	_ret = operand._ret;
	_line = operand._line;
	_lastLineRead = operand._lastLineRead;
	_lastLineSent = operand._lastLineSent;
	_lineString = operand._lineString;
	return (*this);
}

ConfigFile::~ConfigFile()
{
}

/*
** Open the file of the config.
** @param filename The name of the file.
*/
void ConfigFile::openFile(std::string filename)
{
	this->_fd = open(filename.c_str(), O_RDONLY);
	if (_fd < 0)
		throw Exception("Couldn't open file");
}

/*
** Constructor for ConfigFile that opens a file automatically.
** @param filename The name of the file.
*/
ConfigFile::ConfigFile(std::string filename) :
	_lastLineRead(false), _lastLineSent(false)
{
	this->_fd = open(filename.c_str(), O_RDONLY);
	if (_fd < 0)
		throw Exception("Couldn't open file");
}

/*
** Read the next line of the config file.
** Return 0 if the file has ended, so you can use this function in a while loop:
**	while (c.getNext())
**		...
** @ret int Status code.
*/
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
	free(this->_line);
	if (_ret == 0)
	{
		_lastLineRead = true;
		close(_fd);
		return 1;
	}
	return this->_ret;
}

/*
** Return the last read line in a string.
** @ret string The last line.
*/
std::string ConfigFile::getLineString(void) const
{
	if (this->_fd < 0)
		throw Exception("File not open");
	return this->_lineString;
}

/*
** Read the file to the end to prevent future calls to get_next_line from
** bringing up garbage. Use if you raise an exception and plan on reading
** other files.
*/
void ConfigFile::rewind(void)
{
	while (this->getNext())
		continue;
}