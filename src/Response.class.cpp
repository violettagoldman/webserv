#include "Response.class.hpp"

Response::Response(Request req, Location loc, std::string fp)
{
	_fp = fp;
	_req = req;
	_method = _req.getMethod();
	_statusCode = 200;
	statusCodeTranslation();
	_headers["Date"] = getDate(getTime());
	_headers["Server"] = "Webserv/1.0 (Unix)";
	handleMethod(loc);
}

Response::Response(std::string cgi_response)
{
	handleCGI(cgi_response);
}

void Response::handleCGI(std::string cgi_response)
{
	std::cout << "-- CGI --" << std::endl;
	std::cout << cgi_response << std::endl;

	// std::string status = cgi_response;
}

// Response::Response(Response const &src)
// {
// 	*this = src;
// }

Response::~Response(void)
{
}

Response		&Response::operator=(Response const &src)
{
	if (this != &src)
	{
		this->_req = src._req;
		this->_body = src._body;
		this->_headers = src._headers;
		this->_method = src._method;
		this->_statusCode = src._statusCode;
		this->_statusCodeTranslation = src._statusCodeTranslation;
	}
	return (*this);
}

void			Response::handleMethod(Location loc)
{
	std::string option;
	(void)loc;

	_errorPages = loc.getErrorPage();
	option = _method;
	if (_req.getError() != 0)
	{
		if (_req.getError() == 405)
			options(loc);
		error(_req.getError());
		return;
	}
	if (option == "GET")
		get(loc);
	else if (option == "POST")
		post();
	else if (option == "PUT")
		put();
	else if (option == "DELETE")
		deleteMethod();
	else if (option == "OPTIONS")
		options(loc);
	else if (option == "CONNECT")
		connect();
	else if (option == "TRACE")
		trace();
	else if (option == "HEAD")
		head(loc);
	else
	{
		options(loc);
		error(405);
	}
}

void		Response::get(Location loc)
{
	bool			autoindex;
	std::string		path;
	struct stat		fileStat;
	int				fd;

	path = _fp;
	autoindex = loc.getAutoindex();
	fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
	{
		error(404);
		setContentType(".html");
		close(fd);
		return ;
	}
	fstat(fd, &fileStat);
	if (S_ISDIR(fileStat.st_mode))
	{
		bool is_index = false;
		close(fd);
		for (size_t i = 0; i < loc.getIndex().size(); ++i)
		{
			std::string index = path + loc.getIndex()[i];
			int fd_try = open(index.c_str(), O_RDONLY);
			if (fd_try >= 0)
			{
				close(fd);
				path = index;
				is_index = true;
			}
		}
		if (!is_index)
		{
			if (autoindex)
				setIndexPage(loc);
			else
				error(404);
			return ;
		}
	}
	_body = readFile(path);
	_headers["Last-Modified"] = getDate(fileStat.st_mtime);
	setLastModified(fd);
	setContentType(path);
	close(fd);
}

void		Response::head(Location loc)
{
	get(loc);
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

	// check for customised pages
	if (_errorPages.count(_statusCode) > 0)
		html = readFile(_errorPages[_statusCode]);
	else
		html = readFile("./pages/error.html");
	html = replacehtml(html, "$1", ft_itoa(_statusCode));
	html = replacehtml(html, "$2", _statusCodeTranslation[_statusCode]);
	_body = html;
}

void		Response::setIndexPage(Location loc)
{
	std::string		html;
	std::string		li;
	DIR				*currentDirectory;
	struct dirent 	*dir;

	html = readFile("./pages/index.html");
	html = replacehtml(html, "$1", _req.getPath());
	currentDirectory= opendir(loc.getRoot().c_str());
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
	std::string path;

	fd = -1;
	path = _fp;
	exist = checkPathExistance(path);
	if (exist == 1)
	{
		if ((fd = open(path.c_str(), O_WRONLY | O_APPEND, 0644)) > 0)
		{
			if (write(fd, _req.getBody().c_str(), _req.getBody().length()) == -1)
				error(500);
			else
			{
				_headers["Content-Location"] = _req.getPath();
				_statusCode = 200;
				_body = _req.getBody();
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
	std::string path = _fp;
	exist = checkPathExistance(path);
	if (exist == 1)
	{
		if ((fd = open(path.c_str(), O_WRONLY | O_TRUNC, 0644)) > 0)
		{
			if (write(fd, _req.getBody().c_str(), _req.getBody().length()) == -1)
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
			if (write(fd, _req.getBody().c_str(), _req.getBody().length()) == -1)
				error(500);
			else
			{
				_statusCode = 201;
				_headers["Location"] = _req.getPath();
			}
		}
		else
			error(500);
	}
	close (fd);
}

void		Response::deleteMethod()
{
	std::string path;
	int	fd;

	path = _fp;
	if ((fd = open(path.c_str(), O_RDONLY)) < 0)
	{
		error(404);
		return ;
	}
	if (unlink(path.c_str()) == 0)
		_statusCode = 204;
	else
		error(403);
	close(fd);
	
}

void		Response::options(Location loc)
{
	std::string		methods = "";

	if (loc.getLimitExcept().getMethods().size() == 0)
		methods = "GET, POST, PUT, DELETE, CONNECT, TRACE, OPTIONS";
	else
	{
		for (size_t i = 0; i < loc.getLimitExcept().getMethods().size(); ++i)
		{
			methods += loc.getLimitExcept().getMethods()[i];
			if (i < loc.getLimitExcept().getMethods().size() - 1)
				methods += ", ";
		}
	}
	_headers["Allow"] = methods;
	_statusCode = 200;
}

void		Response::connect()
{
	_statusCode = 200;
}

void		Response::trace()
{
	std::string response = "";
	for (size_t i = 0; i < _req.getHeaders().size(); ++i)
	{
		response += _req.getHeaders()[i].getName() + ": " + _req.getHeaders()[i].getValue()[0];
		response += "\r\n";
	}
	response += "\r\n";
	response += _req.getBody();
	_body = response;
	_statusCode = 200;
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
		_headers["Content-Length"] = ft_itoa(_body.length());
	res = "HTTP/1.1 " + ft_itoa(_statusCode) + " " + _statusCodeTranslation[_statusCode] + "\r\n";
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
	{
		res += it->first + ": " + it->second + "\r\n";
	}
	res += "\r\n";
	if (_method != "HEAD")
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