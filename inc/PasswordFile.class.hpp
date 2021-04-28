/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PasswordFile.class.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:23:55 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/19 17:24:27 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASSWORDFILE_CLASS_HPP
# define PASSWORDFILE_CLASS_HPP

# include <iostream>
# include <string>
# include <vector>
# include "ConfigFile.class.hpp"
# include "Utility.hpp"
# include "Exception.class.hpp"

typedef struct s_credential
{
	std::string username;
	std::string password;
}	Credential;

class PasswordFile
{
public:
	PasswordFile(std::string filename);
	PasswordFile(const PasswordFile &copy);
	PasswordFile &operator= (const PasswordFile &operand);
	~PasswordFile();
	std::vector<Credential> getCredentials(void) const;

private:
	PasswordFile(void);
	std::string _filename;
	ConfigFile _confFile;
	std::vector<Credential> _credentials;
};

#endif