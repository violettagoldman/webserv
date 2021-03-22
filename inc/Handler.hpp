#ifndef HANDLER_HPP
# define HANDLER_HPP

# include "Request.class.hpp"
# include "Config.class.hpp"


std::string	handler(Request req, Config conf);
Location	handlerGetLocation(Request req, Config conf);

#endif