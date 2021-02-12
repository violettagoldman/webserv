/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfigReader.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACONFIGREADER_HPP
# define ACONFIGREADER_HPP

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
# include "VirtualHost.class.hpp"
# include "Location.class.hpp"
# include "Config.class.hpp"
# include "LimitExcept.class.hpp"
# include "Exception.class.hpp"

# include "ConfigFile.class.hpp"
# include "ConfigBlock.class.hpp"
// # include "ABlock.class.hpp"
# include "ServerBlock.class.hpp"

class AConfigReader
{
public:
	AConfigReader(std::string filename);

	ConfigBlock *createConfig();

private:
	// int fd;
	// int ret;
	// char *line;

	ConfigFile confFile;

	ConfigBlock *configBlock;
	std::string lineString;

	// virtualHostPrototype vhp;
	// locationPrototype lp;
	// configPrototype cp;
	// limitExceptPrototype lep;
	std::string lastLineParsed;

	void parse(std::string line);

	std::vector<ServerBlock> serverBlockVector;
};

#endif 