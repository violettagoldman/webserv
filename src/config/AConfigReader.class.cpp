/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfigReader.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:10:39 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:10:43 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AConfigReader.class.hpp"

void throwexp2(std::string msg)
{
	std::cout << "Exception: " << msg << std::endl;
	throw Exception(msg);
}

/*
** Parse the file, reading server blocks into virtualHost objects in
** virtualHostVector member.
*/
void AConfigReader::parse(std::string lineString)
{
	// std::cout << "Parse: " << lineString << std::endl;


	configBlock = new ConfigBlock(confFile);

	configBlock->handle();
	// if (lineString.find("server") != std::string::npos)
	// {
	// 	ServerBlock sb(this->confFile);
	// 	sb.handle();

	// 	serverBlockVector.push_back(sb);
	// 	// ABlock(this->confFile);

	// }
	// size_t needle;
	// if (lineString.find("server") != std::string::npos)
	// {
	// 	this->parse_server();
	// }
	// else if ((needle = lineString.find(
	// 			"client_max_body_size")) != std::string::npos)
	// {
	// 	cp.clientMaxBodySize = parse_size(needle+20, lineString);
	// }
	// else if ((needle = lineString.find("autoindex")) != std::string::npos)
	// {
	// 	cp.autoindex = parseBoolDirective(getDirective(needle+9, lineString));
	// }
	// else if ((needle = lineString.find("index")) != std::string::npos)
	// {
	// 	cp.index = split(getDirective(
	// 		needle+5, lineString), ' ');
	// }
	// else if ((needle = lineString.find("root")) != std::string::npos)
	// {
	// 	cp.root = getDirective(needle+4, lineString);
	// }
}

AConfigReader::AConfigReader(std::string filename) :
	confFile(filename)
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
		parse(confFile.getLineString());
	}

}

ConfigBlock *AConfigReader::createConfig()
{
	return configBlock;
}