/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGREADER_HPP
# define CONFIGREADER_HPP

# include <string>
# include <vector>
# include <map>

# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <iostream>
# include <algorithm>


# include "get_next_line.h"
# include "Exception.class.hpp"

# include "ConfigFile.class.hpp"
# include "Config.class.hpp"
// # include "ABlock.class.hpp"
# include "VirtualHost.class.hpp"

class ConfigReader
{
public:
	ConfigReader(std::string filename);

	Config *createConfig();

private:
	// int fd;
	// int ret;
	// char *line;

	ConfigFile confFile;

	Config *configBlock;
	std::string lineString;

	// virtualHostPrototype vhp;
	// locationPrototype lp;
	// configPrototype cp;
	// limitExceptPrototype lep;
	std::string lastLineParsed;

	void parse();

	std::vector<VirtualHost> serverBlockVector;
};

#endif 