/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PasswordFile.class.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:46:22 by ashishae          #+#    #+#             */
/*   Updated: 2021/03/27 16:34:36 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PasswordFile.class.hpp"

PasswordFile::PasswordFile(std::string filename) :
	_filename(filename),
	_confFile(ConfigFile(filename)),
	_credentials(std::vector<Credential>())
{
	do
	{
		std::string ls = _confFile.getLineString();
		

		if (ls.size() == 0 || (ls.size() > 1 && ls[0] == '#'))
			continue;

		std::vector<std::string> content = ft_split(ls, ':');

		if (content.size() < 2 || content.size() > 3)
			throw Exception("Wrong PasswordFile format.");

		Credential newCred;
		newCred.username = content[0];
		newCred.password = content[1];

		_credentials.push_back(newCred);


		// checkLine(ls);

		// if (ls.find("}") != std::string::npos)
		// {
		// 	blockClosed = true;
		// 	break;
		// }

		// handleLineCommon(ls);
		// handleLine(ls);

	}
	while(_confFile.getNext());
}

std::vector<Credential> PasswordFile::getCredentials(void) const
{
	return _credentials;
}
