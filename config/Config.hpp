/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 18:04:06 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/20 18:42:03 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "VirtualHost.hpp"
# include "Location.hpp"

typedef struct sConfigPrototype
{
	std::vector<VirtualHost> virtualHostVector;
	int clientMaxBodySize;
	bool autoindex;
}			configPrototype;

class Config {

public:
	// Config(std::string filename);
	Config(configPrototype);

	std::vector<VirtualHost> getVirtualHostVector(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;

private:
	std::vector<VirtualHost> virtualHostVector;
	int clientMaxBodySize;
	bool autoindex;
};

#endif