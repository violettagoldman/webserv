/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:55 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/21 18:14:11 by ashishae         ###   ########.fr       */
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

# include <string>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <iostream>
# include <cstdlib>

# include "get_next_line/get_next_line.hpp"
# include "VirtualHost.hpp"
# include "Location.hpp"
# include "Config.hpp"
# include "LimitExcept.hpp"


std::vector<std::string> split(std::string s, char c);

void trimWhitespace(std::string &s);

class Reader {

public:
	Reader(std::string filename);
	//~Reader();
	//Reader(const Reader &copy);
	//Reader &operator= (const Reader &operand);
	Config *createConfig();

private:
	int fd;
	int ret;
	char *line;

	std::string lineString;

	virtualHostPrototype vhp;
	locationPrototype lp;
	configPrototype cp;
	limitExceptPrototype lep;
	
	void parse();
	void parse_server();
	void parse_server_line();
	void parse_location();
	void parse_location_line();
	void parse_listen(size_t needle);
	void resetVirtualHostPrototype();
	void resetLocationPrototype();
	void parse_limit_except();
	void resetLimitExceptPrototype();
	void parse_limit_except_line();

	
};

#endif