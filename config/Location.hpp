/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:18:20 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/03 15:14:04 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <vector>
# include <exception>

# include "DirectiveMap.hpp"

class Location {

public:
	// Location();
	Location(std::string pattern, std::vector<std::string> block);
	// Location(std::string pattern, std::string action_type, std::string value);
	~Location();
	
	// Location(const Location &copy);
	// Location &operator= (const Location &operand);

	std::string getPattern(void) const;
	std::string getRoot(void) const;

	class DirectiveNotFound: public std::exception
	{
		public:
		virtual const char* what() const throw();
	};

private:
	std::string pattern;
	std::string root;

};

#endif