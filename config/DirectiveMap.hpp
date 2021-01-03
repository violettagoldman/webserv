/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveMap.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/02 17:05:51 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/02 17:30:01 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVEMAP_HPP
# define DIRECTIVEMAP_HPP

# include <map>
# include <string>
# include <vector>
# include <iostream>

typedef struct Directive {
	std::string title;
	std::string value;
} directive;

typedef std::map<std::string, std::string> dirmap;

class DirectiveMap {

public:
	DirectiveMap(std::vector<std::string>);
	// DirectiveMap();
	// ~DirectiveMap();
	// DirectiveMap(const DirectiveMap &copy);
	// DirectiveMap &operator= (const DirectiveMap &operand);

	dirmap getMap(void) const;

private:
	dirmap map;

};

#endif