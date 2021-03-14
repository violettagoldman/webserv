#ifndef HANDLER_HPP
# define HANDLER_HPP

# include "Request.class.hpp"
# include "Config.class.hpp"


std::string	handler(Request req, Config conf, VirtualHost vh);
Location	handlerGetLocation(Request req, VirtualHost conf);

#endif