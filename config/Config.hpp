/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:33 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/03 15:08:28 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>
# include <string>
# include <iostream>
# include <exception>
# include <ctype.h>

# include "Location.hpp"
# include "DirectiveMap.hpp"

typedef struct LocationBlock {
	std::string pattern;
	std::vector<std::string> block;
} locationBlock;


// Global configuration object

class Config {

public:
	// Config(int listenIp, std::vector<std::string> serverName,
	// 	std::vector<Location> locations);
	Config(std::vector<std::string> configBlock);
	// Config();
	~Config();
	// Пока сойдут implicit-версии
	// Config(const Config &copy);
	// Config &operator= (const Config &operand);
	int getListenIp(void) const;
	std::vector<std::string> getServerName(void) const;
	std::vector<Location> getLocations(void) const;

	class DirectiveNotFound: public std::exception
	{
		public:
		virtual const char* what() const throw();
	};
	

private:

	void readDirectives(std::vector<std::string> block);
	void readLocations(std::vector<std::string> block);
	std::string read_or_throw(std::string key);
	
	
	int listenIp;
	dirmap directives;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
};

#endif