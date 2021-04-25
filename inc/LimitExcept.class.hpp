/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.class.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMITEXCEPT_CLASS_HPP
# define LIMITEXCEPT_CLASS_HPP

# include "ABlock.class.hpp"
# include "Utility.hpp"

/*
** The block that represents the limit_except directive.
*/
class LimitExcept : public ABlock
{
public:
	LimitExcept(ConfigFile *confFile);

	LimitExcept();
	LimitExcept(const LimitExcept &copy);
	LimitExcept &operator=(const LimitExcept &operand);
	~LimitExcept();


	void handleLine(std::string lineString);

	static std::vector<std::string> parseMethods(std::string line);
	
	std::vector<std::string> getAllow(void) const;
	std::vector<std::string> getDeny(void) const;
	std::vector<std::string> getMethods(void) const;

	bool isEmpty(void) const;


private:
	std::vector<std::string> _allow;
	std::vector<std::string> _deny;
	std::vector<std::string> _methods;

	bool _empty;
};

#endif