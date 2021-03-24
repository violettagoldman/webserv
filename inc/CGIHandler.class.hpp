/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 20:21:51 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/07 15:10:56 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <unistd.h>
# include <vector>
# include <string>
# include <iostream>
# include <sys/errno.h>
# include <sys/wait.h>
# include "get_next_line.h"
# include "ICGIRequest.class.hpp"
# include "Exception.class.hpp"
# include "Request.class.hpp"
# include "Utility.hpp"
# include "Base64.class.hpp"

/* A structure containing all information needed for CGI to be launched.
** Most member names correspond to meta-variables from the CGI spec.
** Those that don't, have their own comment.
*/
typedef struct s_CGIRequest {
	std::string remoteAddr;
	// std::string remoteHost;
	std::string authType;
	std::string remoteIdent;
	std::string remoteUser;
	std::string contentType;
	std::string pathInfo;
	std::string pathTranslated;
	std::string queryString;
	std::string requestMethod;
	std::string requestURI; // Present in the 42 subject, but not in RFC.
	std::string serverPort;
	std::string serverName;
	std::string scriptFilename;
	std::string pathToCGI; // The path to the CGI binary (either php-cgi or other).
}	CGIRequest;

/*
** All the meta-variables that can't be deduced from the Request object.
** Most member names correspond to meta-variables from the CGI spec.
** Those that don't, have their own comment.
*/
typedef struct s_CGIRequires {
	std::string scriptName;
	std::string remoteAddr;
	std::string requestURI; // Present in the 42 subject, but not in RFC.
	std::string serverPort;
	std::string serverName;
	std::string pathToCGI; // The path to the CGI binary (either php-cgi or other).
}	CGIRequires;

/*
** The result of parsing an Authorization header.
*/
typedef struct s_authResult {
	std::string authType; // Basic
	std::string user; // corresponds to remoteUser
	std::string password; // corresponds to remoteIdent
}	authResult;

/*
** The result of parsing the Request path and the script path into the
** variables required by the CGI spec.
*/
typedef struct s_pathResult {
	std::string pathInfo;
	std::string pathTranslated;
	std::string queryString;
}	pathResult;

int	ft_atoi_base(const char *str, const char *base);
char **create_envp(std::vector<std::string> mvars);

/*
** CGIHandler takes in a request and a path to a CGI script. It creates
** all the meta-variables required by the CGI specification, and launches
** the CGI executable. Then it reads the response and returns it as a string
*/
class CGIHandler {

public:
	// This constructor is mostly for testing. It allows you to set all the 
	// variables in the structure and pass the body directly.
	CGIHandler(std::string body, CGIRequest cr);

	// This is the constructor to use in the general case.
	CGIHandler(Request icr, CGIRequires cr);

	// ~CGIHandler();
	// CGIHandler(const CGIHandler &copy);
	// CGIHandler &operator= (const CGIHandler &operand);

	std::string getCgiResponse(void) const;

	static authResult parseAuth(std::string authHeader);
	static pathResult parsePath(std::string requestURI, std::string scriptName);
	static std::string urldecode(std::string encodedString);

private:

	std::string _cgiResponse;
	CGIRequest _cgiRequest;
	int _bodySize;
	int _pipeIn[2];
	int _pipeOut[2];
	char **_envp;

	void readCgiResponse(int fd);
	void writeBodyString(int fd, std::string body);
	void countBodySize(std::string s);
	void launch_cgi(void);
	void execute_cgi(void);
	void openPipes(void);
	void prepareEnvp(void);
	void handleCgi(void);
	void pipeline(std::string body);
};

#endif