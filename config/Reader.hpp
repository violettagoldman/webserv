/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:32:55 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/03 15:16:58 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_HPP
# define READER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <map>
# include <exception>

# include "Config.hpp"

# include "Location.hpp"

typedef std::vector<std::string> strarr;


class Reader {

public:
	Reader(std::string filename);
	//~Reader();
	//Reader(const Reader &copy);
	//Reader &operator= (const Reader &operand);
	// std::string read_or_throw(std::string key);
	std::vector<Config> getConfigVector(void) const;

	class SyntaxError: public std::exception
	{
		public:
		virtual const char* what() const throw();
	};
	

private:
	std::vector<Config> configVector;
	std::vector<std::string> lines;
	std::vector<std::string> currentBlock;
	dirmap currentDirectives;
	int get_block(std::ifstream &myfile);
	
	

};

#endif