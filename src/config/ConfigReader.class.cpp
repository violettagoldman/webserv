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

void throwexp2(std::string msg)
{
	std::cout << "Exception: " << msg << std::endl;
	throw Exception(msg);
}

/*
** Parse the file, reading server blocks into virtualHost objects in
** virtualHostVector member.
*/
void ConfigReader::parse()
{
	// std::cout << "Parse: " << lineString << std::endl;


	// configBlock = new Config(confFile);
	// _config = Config(confFile)
	// configBlock->handle();
	// if (lineString.find("server") != std::string::npos)
	// {
	// 	ServerBlock sb(this->confFile);
	// 	sb.handle();

	// 	serverBlockVector.push_back(sb);
	// 	// ABlock(this->confFile);

	_config.handle();
	configBlock = &_config;


}

ConfigReader::ConfigReader(std::string filename) :
	confFile(filename), _config(confFile)
{
	// lastLineParsed = "";
	// fd = open(filename.c_str(), O_RDONLY);
	// if (fd < 0)
	// 	throwexp2("Couldn't open file");

	// while ((ret = fd_get_next_line(fd, &line)))
	// {
	// 	// std::cout << "Line read: " << line << std::endl;
	// 	// assignLineString();
	// 	this->lineString.assign(line);
	// 	parse(this->lineString);
	// }
	// this->confFile = ConfigFile(filename);
	while (confFile.getNext())
	{
		confFile.getLineString();
		parse();
	}
	//check
	_config.check();

}

Config *ConfigReader::createConfig()
{
	return configBlock;
}

Config ConfigReader::getConfig(void) const
{
	return this->_config;
}