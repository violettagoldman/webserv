/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExcept.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:18:20 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/21 22:15:18 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMITEXCEPT_HPP
# define LIMITEXCEPT_HPP

# include <string>
# include <vector>
# include <exception>

// std::string read_directive(std::string block, std::string key);

typedef struct sLimitExceptPrototype
{
	std::vector<std::string> allow;
	std::vector<std::string> deny;
	std::string method;
}	limitExceptPrototype;



class LimitExcept {

public:
	LimitExcept();
	LimitExcept(std::vector<std::string> allow,
					std::vector<std::string> deny, std::string method);
	LimitExcept(limitExceptPrototype lep);
	// LimitExcept(LimitExceptPrototype lp);
	// ~LimitExcept();

	std::vector<std::string> getAllow(void) const;
	std::vector<std::string> getDeny(void) const;
	std::string getMethod(void) const;

private:
	std::vector<std::string> allow;
	std::vector<std::string> deny;
	std::string method;

};

#endif