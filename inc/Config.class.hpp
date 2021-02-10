/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 18:04:06 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/22 12:35:27 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_CLASS_HPP
# define CONFIG_CLASS_HPP

# include "VirtualHost.class.hpp"
# include "Location.class.hpp"
# include "Exception.class.hpp"

typedef struct sConfigPrototype
{
	std::vector<VirtualHost> virtualHostVector;
	int clientMaxBodySize;
	bool autoindex;
	std::vector<std::string> index;
}			configPrototype;

class Config {

public:
	// Config(std::string filename);
	Config(configPrototype);

	std::vector<VirtualHost> getVirtualHostVector(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;
	std::vector<std::string> getIndex(void) const;

private:
	std::vector<VirtualHost> virtualHostVector;
	int clientMaxBodySize;
	bool autoindex;
	std::vector<std::string> index;
};

#endif