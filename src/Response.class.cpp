#include "Response.class.hpp"

Response::Response(void)
{
	//test values
	_method = "POST";
	_statusCode = 200;
	statusCodeTranslation();
	_headers["Date"] = getDate(getTime());
	_headers["Server"] = "Webserv/1.0 (Unix)";
	std::cout << "Default constructor called\n";
	// setIndexPage();
	// setErrorPage();
	handleMethod();
}

Response::Response(Response const &src)
{
	std::cout << "Copy constructor called\n";
	*this = src;
}

Response::~Response(void)
{
	std::cout << "Destructor called\n";
}

Response		&Response::operator=(Response const &src)
{
	std::cout << "Assignation operator called\n";
	if (this != &src)
	{
	}
	return (*this);
}

void			Response::handleMethod()
{
	std::string option;

	// check for method from the request
	// and check if they are in my config -> otherwise 405
	option = _method;
	if (option == "GET")
		get();
	else if (option == "POST")
		post();
	else if (option == "PUT")
		put();
	else if (option == "DELETE")
		deleteMethod();
	else if (option == "OPTIONS")
		options();
	else if (option == "CONNECT")
		connect();
	else if (option == "TRACE")
		trace();
	else
		error(405);
		// return error 405 if not exist or not allowed
}

void		Response::get()
{
	bool			autoindex;
	std::string		path;
	struct stat		fileStat;
	int				fd;

	path = "/tmp/servtest/a.jpeg"; // get path from request
	autoindex = false;
	if ((fd = open(path.c_str(), O_RDONLY)) < 0)
	{
		error(404);
		return ;
	}
	if (fstat(fd, &fileStat) < 0)
	{
		error(403);
		close(fd);
		return ;
	}
	if (!(fileStat.st_mode & S_IREAD))
	{
		error(403);
		close(fd);
		return ;
	}
	if (S_ISDIR(fileStat.st_mode))
	{
		if (autoindex)
			setIndexPage();
		else
			error(403);
	}
	else
	{
		_body = readFile(path);
		// _headers["Last-Modified"] = getDate(fileStat.st_mtime);
		setLastModified(fd);
		setContentType(path);
	}
	close(fd);
	// if no autoindex and it's a folder => 403 - done
	// if yes => 200 and page with index - done
	// if exists => 200 and send body (last-modified, content type)
	// if doesn't exist => 404 and send page with error - done
	// no rights => 403 - done
}

void		Response::setContentType(std::string path)
{
	std::string							extension;
	size_t								n;
	std::map<std::string, std::string>	m;

	if ((n = path.find_last_of(".")) == std::string::npos)
	{
		_headers["Content-Type"] = "text/html";
		return ;
	}
	extension = std::string(path, n + 1, path.size() - n);
	m["aac"] = "audio/aac";
	m["abw"] = "application/x-abiword";
	m["arc"] = "application/octet-stream";
	m["avi"] = "video/x-msvideo";
	m["azw"] = "application/vnd.amazon.ebook";
	m["bin"] = "application/octet-stream";
	m["bmp"] = "image/bmp";
	m["bz"] = "application/x-bzip";
	m["bz2"] = "application/x-bzip2";
	m["csh"] = "application/x-csh";
	m["css"] = "text/css";
	m["csv"] = "text/csv";
	m["doc"] = "application/msword";
	m["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	m["eot"] = "application/vnd.ms-fontobject";
	m["epub"] = "application/epub+zip";
	m["gif"] = "image/gif";
	m["htm"] = "text/html";
	m["html"] = "text/html";
	m["ico"] = "image/x-icon";
	m["ics"] = "text/calendar";
	m["jar"] = "application/java-archive";
	m["jpeg"] = "image/jpeg";
	m["jpg"] = "image/jpeg";
	m["js"] = "application/javascript";
	m["json"] = "application/json";
	m["mid"] = "audio/midi";
	m["midi"] = "audio/midi";
	m["mpeg"] = "video/mpeg";
	m["mpkg"] = "application/vnd.apple.installer+xml";
	m["odp"] = "application/vnd.oasis.opendocument.presentation";
	m["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	m["odt"] = "application/vnd.oasis.opendocument.text";
	m["oga"] = "audio/ogg";
	m["ogv"] = "video/ogg";
	m["ogx"] = "application/ogg";
	m["otf"] = "font/otf";
	m["png"] = "image/png";
	m["pdf"] = "application/pdf";
	m["ppt"] = "application/vnd.ms-powerpoint";
	m["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	m["rar"] = "application/x-rar-compressed";
	m["rtf"] = "application/rtf";
	m["sh"] = "application/x-sh";
	m["svg"] = "image/svg+xml";
	m["swf"] = "application/x-shockwave-flash";
	m["tar"] = "application/x-tar";
	m["tif"] = "image/tiff";
	m["tiff"] = "image/tiff";
	m["ts"] = "application/typescript";
	m["ttf"] = "font/ttf";
	m["vsd"] = "application/vnd.visio";
	m["wav"] = "audio/x-wav";
	m["weba"] = "audio/webm";
	m["webm"] = "video/webm";
	m["webp"] = "image/webp";
	m["woff"] = "font/woff";
	m["woff2"] = "font/woff2";
	m["xhtml"] = "application/xhtml+xml";
	m["xls"] = "application/vnd.ms-excel";
	m["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	m["xml"] = "application/xml";
	m["xul"] = "application/vnd.mozilla.xul+xml";
	m["zip"] = "application/zip";
	m["3gp"] = "audio/3gpp";
	m["3g2"] = "audio/3gpp2";
	m["7z"] = "application/x-7z-compressed";
	if (m.count(extension))
		_headers["Content-Type"] = m[extension];
	else
		_headers["Content-Type"] = "text/html";
}

void		Response::error(int status)
{
	_statusCode = status;
	setErrorPage();
}

void		Response::setErrorPage()
{
	std::string		html;

	html = readFile("./pages/error.html");
	html = replacehtml(html, "$1", ft_itoa(_statusCode));
	html = replacehtml(html, "$2", _statusCodeTranslation[_statusCode]);
	_body = html;
}

void		Response::setIndexPage()
{
	std::string		html;
	std::string		li;
	DIR				*currentDirectory;
	struct dirent 	*dir;

	html = readFile("./pages/index.html");
	html = replacehtml(html, "$1", "/"); // change to path later
	currentDirectory= opendir(".");
	if (currentDirectory)
	{
		while ((dir = readdir(currentDirectory)) != NULL)
			li += "<li>" + std::string(dir->d_name) + "</li>";
		closedir(currentDirectory);
	}
	html = replacehtml(html, "$2", li);
	_body = html;
}

void		Response::post()
{
	int fd;
	int exist;

	fd = -1;
	std::string path = "/tmp/privet";
	exist = checkPathExistance(path);
	if (exist == 1)
	{
		if ((fd = open(path.c_str(), O_WRONLY | O_APPEND, 0644)) > 0)
		{
			if (write(fd, "replace", std::string("replace").length()) == -1)
				error(500);
			else
			{
				_headers["Content-Location"] = "get url from request";
				_statusCode = 200;
				_body = "request body";
			}
		}
		else
			error(500);
	}
	else if (exist == 0)
	{
		error(404);
	}
	close (fd);
}

void		Response::put()
{
	int fd;
	int exist;

	fd = -1;
	std::string path = "/tmp/test_put1";
	exist = checkPathExistance(path);
	if (exist == 1)
	{
		if ((fd = open(path.c_str(), O_WRONLY | O_TRUNC, 0644)) > 0)
		{
			if (write(fd, "replace", std::string("replace").length()) == -1)
				error(500);
			else
				_statusCode = 204;
		}
		else
			error(500);
	}
	else if (exist == 0)
	{
		if ((fd = open(path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644)) > 0)
		{
			if (write(fd, "replace by the request body", std::string("replace by the request body").length()) == -1)
				error(500);
			else
			{
				_statusCode = 201;
				_headers["Location"] = "get url from request";
			}
		}
		else
			error(500);
	}
	close (fd);
}

void		Response::deleteMethod()
{
	std::string path = "/tmp/a"; // get path from request
	int	fd;

	if ((fd = open(path.c_str(), O_RDONLY)) < 0)
	{
		error(404);
		return ;
	}
	// not alowed -> 405
	if (unlink(path.c_str()) == 0)
		_statusCode = 204; // 200 but without body
	else
		error(403);
	close(fd);
	
}

void		Response::options()
{
	// connect to config
	std::string		methods;

	_headers["Allow"] = "GET, PUT, CONNECT";
	_statusCode = 200;
}

void		Response::connect()
{
	_statusCode = 200;
}

void		Response::trace()
{
}

// move to utility
time_t			Response::getTime()
{
	struct timeval	tv;
	struct timezone	tz;

	gettimeofday(&tv, &tz);
	return (tv.tv_sec + tz.tz_minuteswest * 60);
}

std::string		Response::getDate(time_t time)
{
	char 		buffer[33];
	size_t		last;

	last = strftime(buffer, 32, "%a, %d %b %Y %T GMT", localtime(&time));
	buffer[last] = '\0';
	return (std::string(buffer));
}

int			Response::checkPathExistance(std::string path)
{
	struct stat fileStat;
	int exist;
	
	exist = stat(path.c_str(), &fileStat);
	if (exist == 0)
	{
		if (S_ISREG(fileStat.st_mode))
			return (1);
		else
			return (2);
	}
	else
		return (0);
}

void		Response::setLastModified(int fd)
{
	struct stat fileStat;

	fstat(fd, &fileStat);
	_headers["Last-Modified"] = getDate(fileStat.st_mtime);
}

std::string 	Response::serialize()
{
	std::string res;

	if (_method != "CONNECT" && _statusCode != 201 && _statusCode != 204)
		_headers["Content-Length"] = ft_itoa(_body.size());
	res = "HTTP/1.1 " + ft_itoa(_statusCode) + " " + _statusCodeTranslation[_statusCode] + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		res += it->first + ": " + it->second + "\r\n";
	}
	res += "\r\n";
	res += _body;
	return (res);
}

void		Response::statusCodeTranslation()
{
	_statusCodeTranslation[100] = "Continue";
	_statusCodeTranslation[101] = "Switching Protocols";
	_statusCodeTranslation[200] = "OK";
	_statusCodeTranslation[201] = "Created";
	_statusCodeTranslation[202] = "Accepted";
	_statusCodeTranslation[203] = "Non-Authoritative Information";
	_statusCodeTranslation[204] = "No Content";
	_statusCodeTranslation[205] = "Reset Content";
	_statusCodeTranslation[206] = "Partial Content";
	_statusCodeTranslation[300] = "Multiple Choices";
	_statusCodeTranslation[301] = "Moved Permanently";
	_statusCodeTranslation[302] = "Found";
	_statusCodeTranslation[303] = "See Other";
	_statusCodeTranslation[304] = "Not Modified";
	_statusCodeTranslation[305] = "Use Proxy";
	_statusCodeTranslation[307] = "Temporary Redirect";
	_statusCodeTranslation[400] = "Bad Request";
	_statusCodeTranslation[401] = "Unauthorized";
	_statusCodeTranslation[402] = "Payment Required";
	_statusCodeTranslation[403] = "Forbidden";
	_statusCodeTranslation[404] = "Not Found";
	_statusCodeTranslation[405] = "Method Not Allowed";
	_statusCodeTranslation[406] = "Not Acceptable";
	_statusCodeTranslation[407] = "Proxy Authentication Required";
	_statusCodeTranslation[408] = "Request Timeout";
	_statusCodeTranslation[409] = "Conflict";
	_statusCodeTranslation[410] = "Gone";
	_statusCodeTranslation[411] = "Length Required";
	_statusCodeTranslation[412] = "Precondition Failed";
	_statusCodeTranslation[413] = "Payload Too Large";
	_statusCodeTranslation[414] = "URI Too Long";
	_statusCodeTranslation[415] = "Unsupported Media Type";
	_statusCodeTranslation[416] = "Range Not Satisfiable";
	_statusCodeTranslation[417] = "Expectation Failed";
	_statusCodeTranslation[426] = "Upgrade Required";
	_statusCodeTranslation[500] = "Internal Server Error";
	_statusCodeTranslation[501] = "Not Implemented";
	_statusCodeTranslation[502] = "Bad Gateway";
	_statusCodeTranslation[503] = "Service Unavailable";
	_statusCodeTranslation[504] = "Gateway Timeout";
	_statusCodeTranslation[505] = "HTTP Version Not Supported";
}