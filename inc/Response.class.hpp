#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

# include <iostream>
# include <map>
# include <dirent.h>
# include <sys/stat.h>
# include "Utility.hpp"
# include "Config.class.hpp"
# include "Request.class.hpp"
# include "Utility.hpp"

class Response
{
	public:
		Response(void);
		Response(Request req, Location loc, std::string fp);
		Response(std::string cgi_response, Location loc);
		Response(Response const &src);
		~Response(void);


		Response	&operator=(Response const &src);
		void		setErrorPage();
		void		setIndexPage(Location loc);
		std::string	serialize();
		std::string	getDate(time_t time);
		void		handleMethod(Location loc);
		void		get(Location loc);
		void		head(Location loc);
		void		post();
		void		put();
		void		deleteMethod();
		void		options(Location loc);
		void		connect();
		void		trace();
		void		error(int status);
		void		setContentType(std::string path);
		void		statusCodeTranslation();
		int			checkPathExistance(std::string path);
		void		setLastModified(int fd);
		void		handleCGI(Location loc);
		

		
	private:
		int									_statusCode;
		std::string							_method;
		std::string							_cgi_response;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::map<int, std::string>			_statusCodeTranslation;
		Request								_req;
		std::string							_fp;
		std::map<int, std::string>			_errorPages;
};

#endif
