/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:47:52 by ashishae          #+#    #+#             */
/*   Updated: 2020/12/17 17:57:46 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location()
{
	pattern = "/";
	root = "/var/www/";
}

Location::~Location()
{
}

std::string Location::getPattern(void) const
{
	return this->pattern;
}

std::string Location::getRoot(void) const
{
	return this->root;
}


// Location::Location(const Location &copy)
// {
	
// }

// Location::Location &operator= (const Location &operand)
// {
	

// }

