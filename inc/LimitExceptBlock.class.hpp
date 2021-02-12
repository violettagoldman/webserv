/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptBlock.class.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMITEXCEPTBLOCK_CLASS_HPP
# define LIMITEXCEPTBLOCK_CLASS_HPP

# include "ABlock.class.hpp"
// # include "LocationBlock.class.hpp"
# include "Utility.hpp"

class LimitExceptBlock : public ABlock
{
public:
	LimitExceptBlock(ConfigFile &confFile);
	virtual void handleLine(std::string lineString);

	static std::string parseMethod(std::string line);
	
	std::vector<std::string> getAllow(void) const;
	std::vector<std::string> getDeny(void) const;
	std::string getMethod(void) const;


private:
	std::vector<std::string> allow;
	std::vector<std::string> deny;
	std::string method;

};

#endif