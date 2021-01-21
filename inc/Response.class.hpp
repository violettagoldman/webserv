#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

# include <iostream>
# include <map>

class Response
{
	public:
		Response(void);
		// Response(Request req);
		Response(Response const &src);
		~Response(void);


		Response	&operator=(Response const &src);
		std::string serialize() const;

		
	private:
		int									_statusCode;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		const std::map<int, std::string>	_statusCodeTranslation;
};

#endif