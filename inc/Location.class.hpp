/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.class.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:07:37 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:07:39 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CLASS_HPP
# define LOCATION_CLASS_HPP

# include "ABlock.class.hpp"
# include <string>
# include <vector>
# include <map>
# include "Exception.class.hpp"
# include "Utility.hpp"
# include "LimitExcept.class.hpp"

/*
** The class that represents a location context in a NGINX config file.
*/
class Location : public ABlock
{
public:
	Location(ConfigFile &confFile);
	Location(ABlock &ab);

	void handleLine(std::string lineString);

	std::string getPattern(void) const;
	std::string getFcgiPass(void) const;
	std::map<std::string, std::string> getFcgiParams(void) const;
	std::string getUploadStore(void) const;
	LimitExcept getLimitExcept(void) const;
	bool getFcgiSet(void) const;
	bool getRootSet(void) const;
	bool getUploadStoreSet(void) const;

	void setUploadStore(std::string uploadStore);

	void check(void);

private:
	std::string parsePattern(std::string line);
	std::string pattern;
	LimitExcept limitExcept;
	std::string fcgiPass;
	std::map<std::string, std::string> fcgiParams;
	std::string uploadStore;
	bool rootSet;
	bool fcgiSet;
	bool uploadStoreSet;
};

#endif