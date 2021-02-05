/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICGIRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 16:52:48 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/04 18:06:03 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICGIREQUEST_HPP
# define ICGIREQUEST_HPP

# include <string>

class ICGIRequest {

public:

	// ~ICGIRequest();
	ICGIRequest();
	// ICGIRequest(const ICGIRequest &copy);
	// ICGIRequest &operator= (const ICGIRequest &operand);

	std::string getRemoteAddr() const;
	std::string getRemoteHost() const;
	std::string getRemoteIdent() const;
	std::string getRemoteUser() const;
	std::string getContentType() const;
	std::string getPathInfo() const;
	std::string getPathTranslated() const;
	std::string getQueryString() const;
	std::string getRequestMethod() const;
	std::string getRequestURI() const;
	std::string getServerPort() const;
	std::string getServerName() const;
	std::string getScriptFilename() const;

	// virtual ~ICGIRequest() = 0;

private:
	std::string _remoteAddr;
	std::string _remoteHost;
	std::string _remoteIdent;
	std::string _remoteUser;
	std::string _contentType;
	std::string _pathInfo;
	std::string _pathTranslated;
	std::string _queryString;
	std::string _requestMethod;
	std::string _requestURI;
	std::string _serverPort;
	std::string _serverName;
	std::string _scriptFilename;


};

#endif