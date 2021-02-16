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


class Location : public ABlock
{
public:
	Location(ConfigFile &confFile);
	void handleLine(std::string lineString);

	void inheritParams(int clientMaxBodySize, bool autoindex, std::string root,
			std::vector<std::string> index, std::string uploadStore);

	void inheritParams(ABlock *parent);

	std::string getPattern(void) const;
	std::string getRoot(void) const;
	int getClientMaxBodySize(void) const;
	bool getAutoindex(void) const;
	std::vector<std::string> getIndex(void) const;
	// LimitExcept getLimitExcept(void) const;


	std::string getFcgiPass(void) const;
	std::map<std::string, std::string> getFcgiParams(void) const;
	std::string getUploadStore(void) const;
	std::vector<LimitExcept> getLimitExceptVector(void) const;

	bool getFcgiSet(void) const;
	bool getRootSet(void) const;
	bool getUploadStoreSet(void) const;

	void check(void);

private:

	static std::string parsePattern(std::string line);

	int clientMaxBodySize;
	bool autoindex;
	std::string pattern;
	std::string root;
	std::vector<std::string> index;
	// LimitExcept limitExcept;

	std::vector<LimitExcept> limitExceptVector;
	std::string fcgiPass;
	std::map<std::string, std::string> fcgiParams;
	std::string uploadStore;
	bool rootSet;
	bool fcgiSet;
	bool uploadStoreSet;
};

#endif