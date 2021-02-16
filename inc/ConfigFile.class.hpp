/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.class.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_CLASS_HPP
# define CONFIGFILE_CLASS_HPP

# include <string>
# include "Exception.class.hpp"
# include <fcntl.h>
# include "get_next_line.h"

class ConfigFile
{
public:
	ConfigFile(std::string filename);
	// void next();
	int getNext();
	std::string getLineString(void) const;

private:
	int fd;
	int ret;
	char *line;

	bool lastLineRead;
	bool lastLineSent;
	std::string lineString;
};

# endif