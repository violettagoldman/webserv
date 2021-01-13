/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:55 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/13 15:54:56 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_HPP
# define READER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <map>
# include <exception>

# include <string>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <iostream>
# include <cstdlib>

# include "get_next_line/get_next_line.hpp"

std::vector<std::string> split(std::string s, char c);

// # define BUFFER_SIZE 64

# include "Config.hpp"

# include "Location.hpp"

typedef struct sConfigPrototype
{
	int listenIp;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
}	configPrototype;

typedef struct sLocationPrototype
{
	std::string pattern;
	std::string root;

}	locationPrototype;

typedef std::vector<std::string> strarr;

class Reader {

public:
	Reader(std::string filename);
	//~Reader();
	//Reader(const Reader &copy);
	//Reader &operator= (const Reader &operand);
	// std::string read_or_throw(std::string key);
	// std::string read_file(std::string filename);
	std::vector<Config> getConfigVector(void) const;
	void trimWhitespace(std::string &s);

	class SyntaxError: public std::exception
	{
		public:
		virtual const char* what() const throw();
	};
	

private:
	int fd;
	int ret;
	char *line;

	std::vector<Config> configVector;
	std::vector<std::string> lines;
	std::vector<std::string> currentBlock;
	dirmap currentDirectives;
	int get_block(std::ifstream &myfile);

	configPrototype cp;
	locationPrototype lp;
	
	void parse();
	void parse_server();
	void parse_server_line();
	void parse_location();
	void parse_location_line();

};

#endif