/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICGIRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 16:52:48 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/07 19:20:41 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICGIREQUEST_HPP
# define ICGIREQUEST_HPP

# include <string>

class ICGIRequest {

public:

	// ~ICGIRequest();
	// ICGIRequest();
	// ICGIRequest(const ICGIRequest &copy);
	// ICGIRequest &operator= (const ICGIRequest &operand);

	std::string getRemoteAddr() const;
	std::string getRemoteHost() const;
	std::string getAuthType() const;
	std::string getRemoteIdent() const;
	std::string getRemoteUser() const;
	std::string getContentType() const;
	std::string getRequestMethod() const;
	std::string getRequestURI() const;
	std::string getServerPort() const;
	std::string getServerName() const;
	std::string getScriptFilename() const;

	// virtual ~ICGIRequest() = 0;

private:

};

#endif