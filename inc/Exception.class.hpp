/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 12:37:22 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/22 13:02:20 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_CLASS_HPP
# define EXCEPTION_CLASS_HPP

# include <string>

class Exception {

public:

	Exception(std::string message);
	~Exception();
	Exception(const Exception &copy);
	Exception &operator= (const Exception &operand);
	const char* what() const throw();

private:
	std::string message;

};

#endif