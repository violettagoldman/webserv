/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 20:21:56 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/04 18:21:09 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

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

static int	count_digits(int n)
{
	int digits;

	if (n == 0)
		return (1);
	digits = n < 0 ? 1 : 0;
	while (n != 0)
	{
		n = n / 10;
		digits++;
	}
	return (digits);
}

char		*ft_itoa(int n)
{
	char			*result;
	int				digits;
	int				i;
	int				stop;
	long long int	nbr;

	digits = count_digits(n);
	i = digits - 1;
	stop = ((n < 0) ? 0 : -1);
	if (n < 0)
		nbr = n == -2147483648 ? 2147483648 : -n;
	else
		nbr = n;
	if (!(result = cppalloc(sizeof(char) * (digits + 1))))
		return (NULL);
	result[digits] = '\0';
	while (i > stop)
	{
		result[i--] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	if (stop == 0)
		result[0] = '-';
	return (result);
}

// TODO redo without free
std::string ft_itostr(int n)
{
	char *s = ft_itoa(n);
	std::string str(s);
	free(s);
	return str;
}

char **create_envp(std::vector<std::string> mvars)
{
	char **ret = new char*[mvars.size()+1];
	for (int i = 0; i < mvars.size(); i++)
	{
		ret[i] = ft_strdup(mvars[i].c_str());
	}
	ret[mvars.size()] = NULL;
	return ret;
}

void execute_cgi(const char *cgi_path, const char *requested_file, char **envp)
{
	//"/Users/ae/spacial-steel/cdng/webserv/cgi/tiny.php"
	// const char *path_to_cgi = "/usr/local/bin/php-cgi";
		// const char *path_to_cgi = "/Users/ae/spacial-steel/cdng/webserv/cgi/reader";
	char * const argv[] = {const_cast<char *>(cgi_path), NULL};

	execve(cgi_path, argv, envp);
}

void launch_cgi(int *pipe_in, int *pipe_out, const char *cgi_path,
				const char *requested_file, char **envp)
{
	close(STDIN_FILENO);
	dup(pipe_in[0]);
	close(STDOUT_FILENO);
	dup(pipe_out[1]);
	execute_cgi(cgi_path, requested_file, envp);
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
	v.push_back("REMOTE_HOST=" + _cgiRequest.remoteHost);
	v.push_back("REMOTE_IDENT=" + _cgiRequest.remoteIdent);
	v.push_back("REMOTE_USER=" + _cgiRequest.remoteUser);
	v.push_back("CONTENT_TYPE=" + _cgiRequest.contentType);
	v.push_back("PATH_INFO=" + _cgiRequest.requestURI);

	v.push_back("PATH_TRANSLATED=");
	v.push_back("QUERY_STRING=");

	v.push_back("REQUEST_METHOD=" + _cgiRequest.requestMethod);
	v.push_back("REQUEST_URI=" + _cgiRequest.requestURI);
	v.push_back("SCRIPT_NAME=" + _cgiRequest.scriptFilename);
	
	v.push_back("SERVER_PORT=80");
	v.push_back("SERVER_NAME=example.com");


	v.push_back("SCRIPT_FILENAME=" + _cgiRequest.scriptFilename);


	v.push_back("SERVER_PROTOCOL=HTTP/1.1");
	v.push_back("SERVER_SOFTWARE=Webserv/1.1");


	v.push_back("GATEWAY_INTERFACE=CGI/1.1");
	v.push_back("REDIRECT_STATUS=200");


	this->envp = create_envp(v);

	// while (*envp)
	// {
	// 	// std::cout << "Envp: " << *envp << std::endl;
	// 	// envp++;
	// }
}

void CGIHandler::handleCgi(void)
{
	pid_t pid;
	extern int errno;
	
	pid = fork();
	
	if (pid == 0)
	{
		// child
		// execute_to_pipe(pipe_res);
		const char *path_to_cgi = "/usr/local/bin/php-cgi";
		// const char *path_to_cgi = "/Users/ae/spacial-steel/cdng/webserv/cgi/print_env";
		const char *requested_file = "/Users/ae/spacial-steel/cdng/webserv/cgi/tiny.php";
		// int s = dup(STDIN_FILENO);
		
		// close(pipe_res[0]);
		// close(pipe_res[1]);c
		launch_cgi(pipe_in, pipe_out, path_to_cgi, requested_file, envp);

		
	}
	waitpid(pid, NULL, 0);
}

CGIHandler::CGIHandler(std::string _requestedFile, std::string body, CGIRequest cr) :
	requestedFile(_requestedFile), _cgiRequest(cr)
{
	countBodySize(body);
	openPipes();
	prepareEnvp();
	// writeBodyString(pipe_in[1], body);
	handleCgi();
	close(pipe_out[1]);
	readCgiResponse(pipe_out[0]);
}

CGIHandler::CGIHandler(std::string _requestedFile, std::vector<std::string> body, CGIRequest cr) :
	requestedFile(_requestedFile), _cgiRequest(cr)
{
	countBodySize(body);
	openPipes();
	prepareEnvp();
	// writeBodyStringVector(pipe_in[1], body);
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
	for (int i = 0; i < vs.size(); i++)
	{
		bodySize += vs[i].size();
	}
}

std::vector<std::string> CGIHandler::getCgiResponse(void) const
{
	return cgiResponse;
}

void CGIHandler::writeBodyString(int fd, std::string body)
{
	write(fd, body.c_str(), body.size());
}

void CGIHandler::writeBodyStringVector(int fd, std::vector<std::string> body)
{
	for (int i = 0; i < body.size(); i++)
		write(fd, body[i].c_str(), body[i].size());
}

void CGIHandler::readCgiResponse(int fd)
{
	char *line;
	int ret;

	while ((ret = get_next_line(fd, &line)))
	{
		this->cgiResponse.push_back(line);
	}
	this->cgiResponse.push_back(line);
}