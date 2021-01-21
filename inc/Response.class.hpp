#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

# include <iostream>
# include <map>
# include "Utility.hpp"

class Response
{
	public:
		Response(void);
		// Response(Request req);
		Response(Response const &src);
		~Response(void);


		Response	&operator=(Response const &src);
		std::string serialize();

		
	private:
		int										_statusCode;
		std::map<std::string, std::string>		_headers;
		std::string								_body;
		std::map<int, std::string>		_statusCodeTranslation;
};

#endif
