/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:06:33 by ashishae          #+#    #+#             */
/*   Updated: 2020/12/17 17:54:03 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>
# include <string>
# include "Location.hpp"

// Global configuration object

class Config {

public:
	Config();
	~Config();
	// Пока сойдут implicit-версии
	// Config(const Config &copy);
	// Config &operator= (const Config &operand);
	int getListenIp(void) const;
	std::vector<std::string> getServerName(void) const;
	std::vector<Location> getLocations(void) const;

private:
	int listenIp;
	std::vector<std::string> serverName;
	std::vector<Location> locations;
};

#endif