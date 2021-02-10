/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICGIRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 16:52:48 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/06 15:32:19 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICGIREQUEST_HPP
# define ICGIREQUEST_HPP

# include <string>
# include "Header.class.hpp"

class ICGIRequest {

public:

	// ~ICGIRequest();
	virtual std::string getMethod(void) = 0; //
	virtual std::string getBody(void) = 0; //
	virtual std::vector<Header *> getHeaders(void) = 0; //
	virtual std::string getQuery(void) = 0;
	virtual std::string getPath(void) = 0;
	virtual std::string getFragment(void) = 0;

	// virtual ~ICGIRequest() = 0;

private:

};

#endif