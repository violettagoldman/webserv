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
	ConfigFile();
	ConfigFile(std::string filename);

	void openFile(std::string filename);

	// void next();
	int getNext();
	std::string getLineString(void) const;
	void rewind(void);

private:
	int _fd;
	int _ret;
	char *_line;

	bool _lastLineRead;
	bool _lastLineSent;
	std::string _lineString;
};

# endif