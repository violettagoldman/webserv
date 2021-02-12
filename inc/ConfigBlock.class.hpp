/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigBlock.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGBLOCK_CLASS_HPP
# define CONFIGBLOCK_CLASS_HPP

# include "ABlock.class.hpp"
# include "ServerBlock.class.hpp"

class ConfigBlock : public ABlock
{
public:
	ConfigBlock(ConfigFile &confFile);
	virtual void handleLine(std::string lineString);
	void handle();

	std::vector<ServerBlock> getVirtualHostVector(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;
	std::vector<std::string> getIndex(void) const;
	std::string getRoot(void) const;

	void check(void);

private:
	std::vector<ServerBlock> virtualHostVector;
	int clientMaxBodySize;
	bool autoindex;
	std::vector<std::string> index;
	std::string root;

};

#endif