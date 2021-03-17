/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 20:21:56 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/07 15:44:26 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.class.hpp"

size_t	ft_strlen(const char *str)
{
	char *start;

	start = (char *)str;
	while (*str)
		str++;
	return ((size_t)(str - start));
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	unsigned int i;
	unsigned int limit;

	i = 0;
	limit = (unsigned int)(dstsize - 1);
	if (!dst || !src)
		return (0);
	if (dstsize == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < limit)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

char *cppalloc(size_t size)
{
	char *ret = new char[size];
	return ret;
}

char	*ft_strdup(const char *s1)
{
	char	*p;
	size_t	len;

	len = ft_strlen(s1);
	if ((p = cppalloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	ft_strlcpy(p, s1, len + 1);
	p[len] = '\0';
	return (p);
}

std::string		ft_itostr(int n)
{
	std::string		result;
	long long int	nbr;

	if (n < 0)
		nbr = n == -2147483648 ? 2147483648 : -n;
	else
		nbr = n;
	do
	{
		result.insert(0, std::string(1, nbr % 10 + '0'));
		nbr /= 10;
	}
	while (nbr != 0);
	if (n < 0)
		result.insert(0, "-");
	return (result);
}

char **create_envp(std::vector<std::string> mvars)
{
	char **ret = new char*[mvars.size()+1];
	for (size_t i = 0; i < mvars.size(); i++)
	{
		ret[i] = ft_strdup(mvars[i].c_str());
	}
	ret[mvars.size()] = NULL;
	return ret;
}

bool isCgiPhpCgi(std::string pathToCGI)
{
	if (pathToCGI == "/usr/local/bin/php-cgi" || pathToCGI.find("php-cgi") != std::string::npos)
		return true;
	return false;
}

void CGIHandler::execute_cgi()
{
	char *reqfile = const_cast<char *>(_cgiRequest.scriptFilename.c_str());
	char *cgiPath = const_cast<char *>(_cgiRequest.pathToCGI.c_str());

	if (isCgiPhpCgi(_cgiRequest.pathToCGI))
		reqfile = NULL;

	char *const argv[] = {cgiPath, reqfile};

	execve(cgiPath, argv, envp);
}

void CGIHandler::launch_cgi()
{
	close(STDIN_FILENO);
	dup(pipe_in[0]);
	close(STDOUT_FILENO);
	dup(pipe_out[1]);
	execute_cgi();
}

void CGIHandler::openPipes(void)
{
	pipe(pipe_in);
	pipe(pipe_out);
}

void CGIHandler::prepareEnvp(void)
{
	std::vector<std::string> v;

	v.push_back("AUTH_TYPE=" + _cgiRequest.authType); // basic / digest (request.Authorization)

	if (bodySize > 0)
		v.push_back("CONTENT_LENGTH=" + ft_itostr(bodySize));

	v.push_back("REMOTE_ADDR=" + _cgiRequest.remoteAddr);
	// v.push_back("REMOTE_HOST=" + _cgiRequest.remoteHost);

	v.push_back("REMOTE_IDENT=" + _cgiRequest.remoteIdent);
	v.push_back("REMOTE_USER=" + _cgiRequest.remoteUser);

	v.push_back("CONTENT_TYPE=" + _cgiRequest.contentType);
	v.push_back("PATH_INFO=" + _cgiRequest.pathInfo);

	v.push_back("PATH_TRANSLATED=" + _cgiRequest.pathTranslated);
	v.push_back("QUERY_STRING=" + _cgiRequest.queryString);

	v.push_back("REQUEST_METHOD=" + _cgiRequest.requestMethod);
	v.push_back("REQUEST_URI=" + _cgiRequest.requestURI);
	v.push_back("SCRIPT_NAME=" + _cgiRequest.scriptFilename);
	
	v.push_back("SERVER_PORT=" + _cgiRequest.serverPort);
	v.push_back("SERVER_NAME=" + _cgiRequest.serverName);


	v.push_back("SCRIPT_FILENAME=" + _cgiRequest.scriptFilename);
	v.push_back("SCRIPT_NAME=" + _cgiRequest.scriptFilename);


	v.push_back("SERVER_PROTOCOL=HTTP/1.1");
	v.push_back("SERVER_SOFTWARE=Webserv/1.1");


	v.push_back("GATEWAY_INTERFACE=CGI/1.1");
	v.push_back("REDIRECT_STATUS=200");


	this->envp = create_envp(v);

}

void CGIHandler::handleCgi(void)
{
	pid_t pid;
	extern int errno;
	
	pid = fork();
	
	if (pid == 0)
	{
		launch_cgi();
	}
	waitpid(pid, NULL, 0);
}

std::vector<std::string> getHeaderByKey(std::vector<Header> hds, std::string key)
{
	for (size_t i = 0; i < hds.size(); i++)
	{
		if (hds[i].getName() == key)
		{
			return hds[i].getValue();
		}
	}
	return std::vector<std::string>();
}

std::string getHeaderStringByKey(std::vector<Header> hds, std::string key)
{
	std::vector<std::string> result = getHeaderByKey(hds, key);

	if (result.size() == 0)
		return "";
	if (result.size() == 1)
		return result[0];
	else
	{
		std::cout << "[CGI] Warning: header by key " << key << " contains too many variables" << std::endl;
		return "";
	}
}

/*
** Parse the Authorization header
** @param authHeader the value of the Authorization header
** @ret authResult the structure containing parsed auth type and credentials
*/
authResult CGIHandler::parseAuth(std::string authHeader)
{
	authResult ret;

	std::vector<std::string> parts = ft_split(authHeader, ' ');

	if (parts.size() != 2)
		throw Exception("Wrong Authorization header format.");

	if (parts[0] == "Basic")
	{
		ret.authType = "Basic";

		std::string decodedCreds = Base64(parts[1]).decode();

		std::vector<std::string> creds = ft_split(decodedCreds, ':');
		if (creds.size() != 2)
			throw Exception("Wrong Basic Auth credentials format.");
		ret.user = creds[0];
		ret.password = creds[1];
	}
	else
	{
		throw Exception("Wrong Authorization scheme.");
	}

	return ret;
}

// std::string parseAuth(std::string authValue)
// {
// 	std::vector<std::string> words = split(authValue, ' ');
// }


CGIHandler::CGIHandler(Request icr, CGIRequires cr)
{

	std::vector<Header> hds = icr.getHeaders();


	// From Request headers
	
	// Auth
	std::string authValue;
	if ((authValue = getHeaderStringByKey(hds, "Authorization")) != "")
	{
		authResult ar = parseAuth(authValue);
		_cgiRequest.authType = ar.authType;
		_cgiRequest.remoteUser = ar.user;
		_cgiRequest.remoteIdent = ar.password;
	}

	_cgiRequest.contentType = getHeaderStringByKey(hds, "Content-Type");

	// TODO
	// _cgiRequest.pathInfo = ;
	// _cgiRequest.pathTranslated = ;
	// _cgiRequest.queryString = ;

	_cgiRequest.requestMethod = icr.getMethod();


	// Passed as parameter from matching phase and request

	// 	_cgiRequest.remoteHost = ; // not present in subject
	_cgiRequest.remoteAddr = cr.remoteAddr;
	_cgiRequest.requestURI = cr.requestURI;
	_cgiRequest.serverPort = cr.serverPort;
	_cgiRequest.serverName = cr.serverName;
	_cgiRequest.scriptFilename = cr.scriptName;
	_cgiRequest.pathToCGI = cr.pathToCGI;

	pipeline(icr.getBody());
}

CGIHandler::CGIHandler(std::string body, CGIRequest cr) : _cgiRequest(cr)
{
	pipeline(body);
}

/*
** Does all the internal work
*/
void CGIHandler::pipeline(std::string body)
{
	countBodySize(body);
	openPipes();
	prepareEnvp();
	writeBodyString(pipe_in[1], body);
	handleCgi();
	close(pipe_out[1]);
	readCgiResponse(pipe_out[0]);
}


void CGIHandler::countBodySize(std::string s)
{
	this->bodySize = s.size();
}

void CGIHandler::countBodySize(std::vector<std::string> vs)
{
	this->bodySize = 0;
	for (size_t i = 0; i < vs.size(); i++)
	{
		bodySize += vs[i].size();
	}
}

std::string CGIHandler::getCgiResponse(void) const
{
	return cgiResponse;
}

void CGIHandler::writeBodyString(int fd, std::string body)
{
	write(fd, body.c_str(), body.size());
}

void CGIHandler::writeBodyStringVector(int fd, std::vector<std::string> body)
{
	for (size_t i = 0; i < body.size(); i++)
		write(fd, body[i].c_str(), body[i].size());
}

void cgi_response_select(int fd)
{
	fd_set rfds;

	FD_ZERO(&rfds);

	FD_SET(fd, &rfds);

	struct timeval tv;

	/* Wait up to five seconds. */

	tv.tv_sec = 5;
	tv.tv_usec = 0;

	int retval = select(fd + 1, &rfds, NULL, NULL, &tv);

	if (retval == -1)
		throw Exception("Error while trying to select() CGI response.");
	else if (retval)
		return ;
	else
		throw Exception("Timeout while trying to read CGI response.");

}

void CGIHandler::readCgiResponse(int fd)
{
	char *respline;
	std::string resplineString;
	int ret;

	cgi_response_select(fd);
	while ((ret = fd_get_next_line(fd, &respline)))
	{
		cgi_response_select(fd);
		resplineString.assign(respline);
		this->cgiResponse += resplineString + "\n";
	}
	// this->cgiResponse += resplineString + "\n";
}