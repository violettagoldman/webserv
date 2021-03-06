#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <vector>
# include <unistd.h>
# include "VirtualHost.class.hpp"
# include "Response.class.hpp"
class Server
{
	public:
		Server(void);
		Server(Server const &src);
		~Server(void);

		struct sockaddr_in		_address;

		Server				&operator=(Server const &src);
		int					setup(VirtualHost conf);
		int					accept(void);
		int					listen(void);
		int					send(int fd, std::string message, Response *response);
		int					close(void);
		std::string			getHost(void) const;
		std::string			getPort(void) const;
		int					getFd(void) const;
		std::vector<int>	getClients(void) const;

		void				addClient(int fd);
		void				removeClient(int fd);
		void				setHost(std::string host);
		void				setPort(std::string port);
		void				setFd(int fd);

	private:
		std::string				_host;
		std::string				_port;
		int						_fd;
		std::vector<int>		_clients;
};

#endif
