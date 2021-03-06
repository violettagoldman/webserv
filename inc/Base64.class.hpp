/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 20:21:51 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/25 17:10:10 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_CLASS_HPP
# define BASE64_CLASS_HPP

# include <unistd.h>
# include <vector>
# include <string>

# include "Exception.class.hpp"

# include "Utility.hpp"

/*
** The class that handles Base64 decoding.
*/
class Base64 {

public:
	Base64();

	Base64(const Base64 &copy);
	Base64 &operator=(const Base64 &operand);
	~Base64();

	Base64(std::string input);
	std::string decode(void);

private:
	std::string _content;
};

#endif