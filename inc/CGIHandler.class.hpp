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


# ifndef PHPCGI_PATH
#  ifdef __APPLE__
#   define PHPCGI_PATH "/usr/local/bin"
#  endif
# endif

typedef struct s_CGIRequest {
	std::string remoteAddr;
	std::string remoteHost;
	std::string authType;
	std::string remoteIdent;
	std::string remoteUser;
	std::string contentType;
	// std::string pathInfo;
	// std::string pathTranslated;
	// std::string queryString;
	std::string requestMethod;
	std::string requestURI;
	std::string serverPort;
	std::string serverName;
	std::string scriptFilename;
	std::string pathToCGI;
}	CGIRequest;


class CGIHandler {

public:
	CGIHandler(std::string body, CGIRequest cr);
	// CGIHandler(ICGIRequest icr, std::string requestedFile);

	// ~CGIHandler();
	// CGIHandler(const CGIHandler &copy);
	// CGIHandler &operator= (const CGIHandler &operand);

	std::string getCgiResponse(void) const;

private:
	std::string requestedFile;
	std::string cgiResponse;

	CGIRequest _cgiRequest;

	int bodySize;

	int pipe_in[2];
	int pipe_out[2];

	char **envp;

	void readCgiResponse(int fd);
	void writeBodyString(int fd, std::string body);
	void writeBodyStringVector(int fd, std::vector<std::string> body);
	void countBodySize(std::string s);
	void countBodySize(std::vector<std::string> vs);

	void launch_cgi(void);

	void execute_cgi(void);

	void openPipes(void);
	void prepareEnvp(void);

	void handleCgi(void);
};

#endif