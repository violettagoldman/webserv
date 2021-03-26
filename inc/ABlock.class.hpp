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
# include <algorithm>
# include "Utility.hpp"

/*
** This is the abstract base class for all other blocks. It handles the
** common directives (such as index, root etc.) and allows you to inherit
** their values in child-level blocks.
*/
class ABlock
{

public:
	ABlock(ConfigFile &confFile);

	virtual void handle();

	static bool blockStarted(std::string lineString);

	void handleLineCommon(std::string lineString);

	virtual void handleLine(std::string lineString);

	ConfigFile &getConfFile(void) const;

	virtual void check(void);

	void insertErrorPages(std::map<int, std::string> &newErrorPages);

	void parseErrorPage(std::string directiveValue);

	static bool isPresent(std::string lineString, std::string keyword);

	static std::string getStringDirective(std::string lineString, std::string key);
	static void trimWhitespace(std::string &s);
	static void trimWhitespaceStart(std::string &s);

	static int parseClientMaxBodySize(std::string lineString);
	static void parseFastCGIParam(std::string lineString,
			std::map<std::string, std::string> &params);
	static bool parseBoolDirective(std::string lineString, std::string key);
	static void parseListen(std::string lineString, std::string &listenHost,
							int &listenPort);

	static int countOccurence(std::string s, char c);

	static void checkLine(std::string lineString);

	virtual ~ABlock();

	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;
	std::vector<std::string> getIndex(void) const;
	std::string getRoot(void) const;
	std::map<int, std::string> getErrorPage(void) const;

private:
	ConfigFile &_confFile;

	int _clientMaxBodySize;
	bool _autoindex;
	std::vector<std::string> _index;
	std::string _root;
	std::map<int, std::string> _errorPage;

};

#endif