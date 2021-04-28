/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigReader.class.hpp"

/*
** Parse the file, reading server blocks into virtualHost objects in
** virtualHostVector member.
*/
void ConfigReader::parse()
{
	_config.handle();
}

ConfigReader::ConfigReader(const ConfigReader &copy) :
	confFile(copy.confFile),
	_config(copy._config),
	lineString(copy.lineString),
	lastLineParsed(copy.lastLineParsed),
	serverBlockVector(copy.serverBlockVector)
{
}

ConfigReader &ConfigReader::operator=(const ConfigReader &operand)
{
	confFile = operand.confFile;
	_config = operand._config;
	lineString = operand.lineString;
	lastLineParsed = operand.lastLineParsed;
	serverBlockVector = operand.serverBlockVector;
	return (*this);
}

ConfigReader::~ConfigReader()
{
}

ConfigReader::ConfigReader(std::string filename) :
	confFile(filename), _config(&confFile)
{
	while (confFile.getNext())
	{
		confFile.getLineString();
		parse();
	}

	// Check the resulting config.
	_config.check();
}

Config ConfigReader::getConfig(void) const
{
	return this->_config;
}