/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ABlock.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 12:37:22 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/22 13:02:20 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABLOCK_CLASS_HPP
# define ABLOCK_CLASS_HPP

#include "ConfigFile.class.hpp"
# include <iostream>

# include <vector>
# include <map>
# include "Utility.hpp"

class ABlock
{



public:
	static const std::string blockStartKeyword;
	ABlock(ConfigFile &confFile);

	virtual void handle();

	static bool blockStarted(std::string lineString);

	virtual void handleLine(std::string lineString);

	ConfigFile &getConfFile(void) const;

	virtual void check(void);

	static bool isPresent(std::string lineString, std::string keyword);

	static std::string getStringDirective(std::string lineString, std::string key);
	static void trimWhitespace(std::string &s);
	static void trimWhitespaceStart(std::string &s);

	static int parseClientMaxBodySize(std::string lineString);
	static void parseFastCGIParam(std::string lineString,
			std::map<std::string, std::string> &params);
	static bool parseBoolDirective(std::string lineString, std::string key);
	static void parseListen(std::string lineString, std::string &listenHost,
							int &listenIp);

	static int countOccurence(std::string s, char c);

	static void checkLine(std::string lineString);
	virtual ~ABlock();

private:
	ConfigFile &_confFile;

};

#endif