#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

# include <iostream>
# include <map>
# include <sys/time.h>
# include <dirent.h>
# include <sys/stat.h>
# include "Utility.hpp"

class Response
{
	public:
		Response(void);
		// Response(Request req);
		Response(Response const &src);
		~Response(void);


		Response	&operator=(Response const &src);
		void		setErrorPage();
		void		setIndexPage();
		std::string	serialize();
		time_t		getTime();
		std::string	getDate(time_t time);
		void		handleMethod();
		void		get();
		void		post();
		void		put();
		void		deleteMethod();
		void		options();
		void		connect();
		void		trace();
		void		patch();
		void		error(int status);
		void		setContentType(std::string path);
		void		statusCodeTranslation();
		int			checkPathExistance(std::string path);
		void		setLastModified(int fd);
		

		
	private:
		int										_statusCode;
		std::string								_method;
		std::map<std::string, std::string>		_headers;
		std::string								_body;
		std::map<int, std::string>				_statusCodeTranslation;
};

#endif
