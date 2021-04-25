/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.class.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/25 17:27:59 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_CLASS_HPP
# define CONFIGFILE_CLASS_HPP

# include <string>
# include "Exception.class.hpp"
# include <fcntl.h>
# include "get_next_line.h"

/*
** An abstraction for reading config files in a while loop, raising file
** exception errors, and closing the file.
*/
class ConfigFile
{
public:
	// Default constructor for ConfigFile. Use openFile then to open a file.
	ConfigFile();

	ConfigFile(const ConfigFile &copy);
	ConfigFile &operator=(const ConfigFile &operand);
	~ConfigFile();

	// Use this constructor normally.
	ConfigFile(std::string filename);


	void openFile(std::string filename);

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