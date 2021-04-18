/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.class.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/15 17:31:48 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGREADER_HPP
# define CONFIGREADER_HPP

# include <string>
# include <vector>

# include "get_next_line.h"
# include "Exception.class.hpp"

# include "ConfigFile.class.hpp"
# include "Config.class.hpp"
# include "VirtualHost.class.hpp"

class ConfigReader
{
public:
	ConfigReader(std::string filename);

	Config getConfig(void) const;

private:
	ConfigFile confFile;
	Config _config;
	std::string lineString;

	std::string lastLineParsed;

	void parse();

	std::vector<VirtualHost> serverBlockVector;
};

#endif 