/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.class.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 20:21:56 by ashishae          #+#    #+#             */
/*   Updated: 2021/04/27 18:38:07 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.class.hpp"

CGIHandler::CGIHandler()
{
}

CGIHandler::CGIHandler(const CGIHandler &copy) :
	_cgiResponse(copy._cgiResponse),
	_cgiRequest(copy._cgiRequest),
	_bodySize(copy._bodySize),
	_envp(copy._envp),
	_useTempFile(copy._useTempFile),
	_tempFileWriteFd(copy._tempFileWriteFd),
	_headers(copy._headers)
{
	_pipeIn[0] = copy._pipeIn[0];
	_pipeIn[1] = copy._pipeIn[1];
	_pipeOut[0] = copy._pipeOut[0];
	_pipeOut[1] = copy._pipeIn[1];
}

CGIHandler &CGIHandler::operator=(const CGIHandler &operand)
{
	_cgiResponse = operand._cgiResponse;
	_cgiRequest = operand._cgiRequest;
	_bodySize = operand._bodySize;
	_envp = operand._envp;
	_useTempFile = operand._useTempFile;
	_tempFileWriteFd = operand._tempFileWriteFd;
	_headers = operand._headers;
	_pipeIn[0] = operand._pipeIn[0];
	_pipeIn[1] = operand._pipeIn[1];
	_pipeOut[0] = operand._pipeOut[0];
	_pipeOut[1] = operand._pipeIn[1];

	return (*this);
}

CGIHandler::~CGIHandler()
{
}

/*
** Return true if the CGI called is php-cgi binary, and false otherwise.
** @param pathToCGI the path to the CGI executable called
*/
bool isCgiPhpCgi(std::string pathToCGI)
{
	if (pathToCGI == "/usr/local/bin/php-cgi" || pathToCGI.find("php-cgi") != std::string::npos)
		return true;
	return false;
}


/*
** Find a header in a vector of headers by key, and return all its values
** @param key The key that has to be equal to the key in header
** @param hds The vector of headers to be searched in
** @ret vector<string> All the values of the header if present, empty vector
** 	if no header was found.
*/
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

/*
** Find a header in a vector of headers by key, and return its only,
** or first value (and print a warning if there is many values).
** This is useful because in Request headers are only splited by ; so
** most of headers will only have 1 value.
** @param key The key to look for.
** @param hds The vector of headers to look in.
** @ret string The value, or an empty string if no header found.
**
*/
std::string getHeaderStringByKey(std::vector<Header> hds, std::string key)
{
	std::vector<std::string> result = getHeaderByKey(hds, key);

	if (result.size() == 0)
		return "";
	if (result.size() == 1)
		return result[0];
	else
	{
		std::cout << "[CGI] Warning: header by key " << key << " contains too many values" << std::endl;
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

/*
** A constructor from a Request and a structure of necessary parameters.
** @param icr The request that was matched to be served by CGI
** @param cr All other necessary information in a structure
*/
CGIHandler::CGIHandler(Request icr, CGIRequires cr) : _useTempFile(false)
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

	pathResult pr = parsePath(cr.requestURI, cr.scriptName);
	_cgiRequest.pathInfo = pr.pathInfo;
	_cgiRequest.pathTranslated = pr.pathTranslated;
	_cgiRequest.queryString = pr.queryString;

	_cgiRequest.requestMethod = icr.getMethod();

	_headers = icr.getHeaders();
	// Passed as parameter from matching phase and request

	// 	_cgiRequest.remoteHost = ; // not present in subject
	_cgiRequest.remoteAddr = cr.remoteAddr;
	_cgiRequest.requestURI = cr.requestURI;
	_cgiRequest.serverPort = cr.serverPort;
	_cgiRequest.serverName = cr.serverName;
	_cgiRequest.scriptFilename = cr.scriptName;
	_cgiRequest.pathToCGI = cr.pathToCGI;
	// std::cout << "Body is: " << icr.getBody() << std::endl;
	// std::cout << "Body size is: " << icr.getBody().size() << std::endl;
	pipeline(icr.getBody());
}

CGIHandler::CGIHandler(std::string body, CGIRequest cr) : _cgiRequest(cr), _useTempFile(false)
{
	pipeline(body);
}

/*
** This is the dispatch function that controls what CGI will do,
** and in what order, to prevent repetition in constructors.
** @param body The request body.
*/
void CGIHandler::pipeline(std::string body)
{
	pid_t pid;
	countBodySize(body);
	// if (body.size() > 6)
	// {
	// 	_useTempFile = true;
	// 	_tempFileWriteFd = open("webservTmp", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	// }
	openPipes();
	
	pid = fork();
	if (pid == 0)
	{
		prepareEnvp();
		launch_cgi();
	}
	close(_pipeIn[0]);
	// int writeTarget = _useTempFile ? _tempFileWriteFd : _pipeIn[1];
	// std::cout << "writeTarget: " << writeTarget << std::endl;

	ssize_t write_status = write(_pipeIn[1], body.c_str(), body.size());
	if (write_status < 0)
		throw Exception("Error while trying to write response body to CGI");

	close(_pipeIn[1]);
	(void)_useTempFile;
	waitpid(pid, NULL, 0);
	// handleCgi();
	// close(_pipeOut[1]);
	readCgiResponse(_tempFileWriteFd);
}

/*
** Count the body size in octets to pass to CGI.
** @param s The request body.
*/
void CGIHandler::countBodySize(std::string s)
{
	_bodySize = s.size();
}

/*
** Create the pipes to write in stdin, and to read from stdout
*/
void CGIHandler::openPipes(void)
{
	// if (!_useTempFile)
	// {
	pipe(_pipeIn);
	// }
	(void)_pipeOut;
	// pipe(_pipeOut);
}

/*
** Create the environment variables that are required by the CGI standard,
** and save them as a vector of strings
*/
void CGIHandler::prepareEnvp(void)
{
	std::vector<std::string> v;
	std::string buf;
	v.push_back("AUTH_TYPE=" + _cgiRequest.authType); // basic / digest (request.Authorization)

	// if (_bodySize > 0)
	v.push_back("CONTENT_LENGTH=" + ft_itostr(_bodySize));

	v.push_back("REMOTE_ADDR=" + _cgiRequest.remoteAddr);
	// v.push_back("REMOTE_HOST=" + _cgiRequest.remoteHost); // not present in the subject

	v.push_back("REMOTE_IDENT=" + _cgiRequest.remoteIdent);
	v.push_back("REMOTE_USER=" + _cgiRequest.remoteUser);

	v.push_back("CONTENT_TYPE=" + _cgiRequest.contentType);
	// v.push_back("CONTENT_TYPE=text/html; charset=utf-8");
	v.push_back("PATH_INFO=" + _cgiRequest.pathInfo);

	v.push_back("PATH_TRANSLATED=" + _cgiRequest.scriptFilename);
	v.push_back("QUERY_STRING=" + _cgiRequest.queryString);

	v.push_back("REQUEST_METHOD=" + _cgiRequest.requestMethod);
	v.push_back("REQUEST_URI=" + _cgiRequest.requestURI);
	v.push_back("SCRIPT_NAME=" + _cgiRequest.pathInfo);

	v.push_back("SERVER_NAME=" + _cgiRequest.serverName);
	v.push_back("SERVER_PORT=" + _cgiRequest.serverPort);

	v.push_back("SCRIPT_FILENAME=" + _cgiRequest.scriptFilename);

	v.push_back("SERVER_PROTOCOL=HTTP/1.1");
	v.push_back("SERVER_SOFTWARE=Webserv/1.1");

	v.push_back("GATEWAY_INTERFACE=CGI/1.1");
	v.push_back("REDIRECT_STATUS=200");
	for (std::vector<Header>::iterator it = _headers.begin(); it < _headers.end(); ++it)
	{
		buf = it->getName();
		if (buf == "Authorization" || buf == "Content-Length" || buf == "Content-Type" || buf == "Host")
			continue;
		for (size_t i = 0; i < buf.length(); ++i)
			buf[i] = std::toupper(buf[i]);
		buf = "HTTP_" + buf;
		std::replace(buf.begin(), buf.end(), '-', '_');
		buf += "=";
		buf += it->getValue()[0];
		// for (std::vector<std::string>::iterator itv = it.getValue().begin(); itv < it.getValue().end(); ++itv)
		// 	buf += *it;
		v.push_back(buf);
	}

	_envp = create_envp(v);
}

/*
** Create an environment pointer
** @param mvars The vector of key=value environment variable strings
** @ret char ** Envp to pass to execve
*/
char **create_envp(std::vector<std::string> mvars)
{
	char **ret = new char*[mvars.size()+1];
	for (size_t i = 0; i < mvars.size(); i++)
	{
		std::cout << mvars[i] << std::endl;
		ret[i] = ft_strdup(mvars[i].c_str());
	}
	ret[mvars.size()] = NULL;
	return ret;
}

bool canExecute(std::string path)
{
	struct stat fileStat;

	return (stat(path.c_str(), &fileStat) == 0 && fileStat.st_mode & S_IXUSR);
}

/*
** Prepare all the pipes (pipe to write in stdin, pipe to read from stdout)
** and launch the CGI binary.
*/
void CGIHandler::launch_cgi()
{
	// close(STDIN_FILENO);
	// if (_useTempFile)
	// {
	// 	int readFd = open("/tmp/webservTmp", O_RDONLY);
	// 	std::cout << "TEMP" << std::endl;
	// 	close(STDIN_FILENO);
	// 	dup(readFd);
	// }
	// else
	// {
		// close(STDIN_FILENO);
	if (!canExecute(_cgiRequest.pathToCGI.c_str()))
		throw Exception("Couldn't find or execute CGI binary");
	
	_tempFileWriteFd = open("webservTmp",O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	close(_pipeIn[1]);
	dup2(_pipeIn[0], 0);
	dup2(_tempFileWriteFd, 1);
	close(_tempFileWriteFd);
	// }
	close(_pipeIn[0]);
	// close(STDOUT_FILENO);
	// dup2(_pipeOut[1]);
	execute_cgi();
	close(0);

}

/*
** Execute the CGI binary, passing the script name as argument
*/
void CGIHandler::execute_cgi()
{
	// char *reqfile = const_cast<char *>(_cgiRequest.scriptFilename.c_str());
	char *cgiPath = const_cast<char *>(_cgiRequest.pathToCGI.c_str());

	// fcntl(_tempFileWriteFd, F_SETFL, O_NONBLOCK);

	char *const argv[] = {cgiPath, NULL, NULL};

	// If cgi interpreter not specified, just launch the thing

	// Php-cgi scheme: only launch php-cgi
	// Apparently, php-cgi doesn't like the script name as argument
	//if (isCgiPhpCgi(_cgiRequest.pathToCGI))
//		argv[0] = cgiPath;
//	else if () // tester scheme,



//	if (_cgiRequest.pathToCGI == "")
		// char *const argv[] = {reqfile, NULL};
//	else
		// char *const argv[] = {cgiPath, reqfile, NULL};


	// Apparently this is not needed

	execve(cgiPath, argv, _envp);
}

/*
** Read the CGI's response from the piped standard output.
** @param fd The file descriptor where the CGI's stdout is piped to.
*/
void CGIHandler::readCgiResponse(int fd)
{
	// char *respline;
	std::string resplineString;
	int ret;
	char buf[BUFFER_SIZE + 1];

	ft_bzero(buf, BUFFER_SIZE + 1);
	fd = open("webservTmp", O_RDONLY, 0666);
	_cgiResponse = "";
	while ((ret = read(fd, buf, BUFFER_SIZE)))
	{
		_cgiResponse.append(buf);
		ft_bzero(buf, BUFFER_SIZE + 1);
	}
	close(fd);
}

/*
** The getter for cgiResponse
*/
std::string CGIHandler::getCgiResponse(void) const
{

	return _cgiResponse;
}

/*
** Parse the path of the request into the segments required by the CGI
** standard (PATH_INFO, PATH_TRANSLATED, QUERY_STRING)
** @param requestURI The full path of the request.
** @param scriptName The matched path to the CGI script in our document root.
*/
pathResult CGIHandler::parsePath(std::string requestURI, std::string scriptName)
{
	pathResult ret;
	size_t scriptNameStart = scriptName.rfind('/');
	std::string scriptFilename = scriptName.substr(scriptNameStart);

	std::string realURI = requestURI;

	if (requestURI.find("http://") != std::string::npos)
	{
		size_t hostPartStart = requestURI.find("http://") + 7;
		size_t URIPartStart = requestURI.find("/", hostPartStart);
		realURI = requestURI.substr(URIPartStart);
	}
	//size_t scriptPosition = requestURI.rfind(scriptFilename);

	//std::string afterScript = requestURI.substr(scriptPosition+scriptFilename.size());

	ret.pathInfo = urldecode(realURI);

	size_t queryStringStart = realURI.find("?");

	if (queryStringStart != std::string::npos)
		ret.queryString = realURI.substr(queryStringStart+1);
	else
		ret.queryString = "";

	std::string localPath = scriptName.substr(0, scriptNameStart);
	ret.pathTranslated = localPath + ret.pathInfo;

	return ret;
}

/*
** Return the index of c in str, and -1 if c is not in str.
** @param str The string to find c in.
** @param c The character to be found in str.
** @ret int The index of c in str, or -1 if c is not in str.
*/
int ft_find(const char *str, const char c)
{
	size_t i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return i;
		i++;
	}
	return -1;
}

/*
** Read the base base integer from str, and return it as an int.
** Skip all whitespace in the beginning of the string.
** Only one - or + is allowed in the beginning.
** Then, an integer followed by a \0.
** @param str The string containing the string in a specified base
** @param base The base in which the integer is in string
** @ret int The value of the said integer.
*/
int	ft_atoi_base(const char *str, const char *base)
{
	int		nbr;
	int		sign;
	int		newval;

	nbr = 0;
	sign = 1;
	while ((*str) == '\t' || (*str) == '\n' || (*str) == '\v' || (*str) == '\f'
			|| (*str) == '\r' || (*str) == ' ')
		str++;
	if ((*str) == '-' || (*str) == '+')
	{
		sign *= ((*str) == '-' ? -1 : 1);
		str++;
	}
	while ((*str) != '\0')
	{
		nbr *= ft_strlen(base);
		newval = ft_find(base, *str);
		if (newval == -1)
			throw Exception("Invalid character in ft_atoi_base.");
		nbr += newval;
		str++;
	}
	return (nbr * sign);
}

/*
** Convert a hexadecimal byte passed as parameter to a character in ASCII.
** @param byte A byte in hexadecimal, either in capitals or not.
** @ret char A corresponding character in ASCII.
*/
char convert(std::string byte)
{
	char res = 0;
	const std::string capitals = "ABCDEF";
	if (capitals.find(byte[0]) != std::string::npos || capitals.find(byte[1]) != std::string::npos)
		res = (char) ft_atoi_base(byte.c_str(), "0123456789ABCDEF");
	else
		res = (char) ft_atoi_base(byte.c_str(), "0123456789abcdef");
	return res;

}

/*
** Decode a percent-encoded string and return it.
** @param encodedString The percent-encoded string.
** @ret string The decoded string.
*/
std::string CGIHandler::urldecode(std::string encodedString)
{
	size_t pos;
	std::string encodedSection;
	char replacement;
	std::string replacementString;

	while ((pos = encodedString.find("%")) != std::string::npos)
	{
		encodedSection = encodedString.substr(pos+1, 2);
		replacement = convert(encodedSection);
		replacementString.clear();
		replacementString.push_back(replacement);
		encodedString.replace(pos, 3, replacementString);

	}
	return encodedString;
}
