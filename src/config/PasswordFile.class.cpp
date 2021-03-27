/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PasswordFile.class.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:46:22 by ashishae          #+#    #+#             */
/*   Updated: 2021/03/27 14:02:50 by ashishae         ###   ########.fr       */
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

		std::cout << ls << std::endl;
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
