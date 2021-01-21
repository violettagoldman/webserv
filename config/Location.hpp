/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:18:20 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/21 18:17:23 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <vector>
# include <exception>
# include "LimitExcept.hpp"

std::string read_directive(std::string block, std::string key);

typedef struct sLocationPrototype
{
	std::string pattern;
	std::string root;
	int clientMaxBodySize;
	bool autoindex;
	std::vector<std::string> index;
	LimitExcept limitExcept;
}	locationPrototype;

class Location {

public:
	// Location();
	// Location(std::string pattern, std::string block);
	// Location(std::string pattern, std::string action_type, std::string value,
		// int clientMaxBodySize, bool autoindex);
	Location(locationPrototype lp);
	~Location();
	
	// Location(const Location &copy);
	// Location &operator= (const Location &operand);

	std::string getPattern(void) const;
	std::string getRoot(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;
	std::vector<std::string> getIndex(void) const;
	LimitExcept getLimitExcept(void) const;

private:
	int clientMaxBodySize;
	bool autoindex;
	std::string pattern;
	std::string root;
	std::vector<std::string> index;
	LimitExcept limitExcept;


};

#endif