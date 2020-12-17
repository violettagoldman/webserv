/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:18:20 by ashishae          #+#    #+#             */
/*   Updated: 2020/12/17 17:57:33 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string> 

class Location {

public:
	Location();
	~Location();
	// Location(const Location &copy);
	// Location &operator= (const Location &operand);

	std::string getPattern(void) const;
	std::string getRoot(void) const;

private:
	std::string pattern;
	std::string root;

};

#endif