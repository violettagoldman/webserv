/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 20:05:05 by ablanar           #+#    #+#             */
/*   Updated: 2021/01/22 01:08:51 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_CLASS_HPP
#define HEADER_CLASS_HPP
#include <vector>
#include <string>
class Header
{
	public:
		Header(std::string name, std::vector<std::string> value);
		Header(Header const &src);
		~Header(void);
		Header &operator=(Header const &src);
		void setError(int i);
		int isError(void);
	private:
		Header(void);
		std::string _name;
		std::vector<std::string> _values;
		int _error;
};

int isValidHeader(std::string name);

#endif
