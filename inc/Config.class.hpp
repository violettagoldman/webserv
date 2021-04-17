/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.class.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_CLASS_HPP
# define CONFIG_CLASS_HPP

# include "ABlock.class.hpp"
# include "VirtualHost.class.hpp"

/*
** Config represents the entire file, and roughly corresponds to the 
** http context in NGINX.
*/
class Config : public ABlock
{
public:
	Config();
	Config(ConfigFile *confFile);
	virtual void handleLine(std::string lineString);
	void handle();
	std::vector<VirtualHost> getVirtualHostVector(void) const;
	void check(void);

private:
	std::vector<VirtualHost> virtualHostVector;

};

#endif