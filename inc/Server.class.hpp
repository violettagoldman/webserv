#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h> // not to use

class Server
{
	public:
		Server(void);
		Server(Server const &src);
		~Server(void);

		struct sockaddr_in		_address;

		Server	&operator=(Server const &src);
		int				setup(void);
		int				run(void);
		int				accept(void);
		int				listen(void);
		std::string		recieve(void);
		int				send(int fd, std::string message) const;
		int				shutdown(void);
		int				close(void);
		std::string		getHost(void) const;
		std::string		getPort(void) const;
		int				getFd(void) const;

		void			setHost(std::string host);
		void			setPort(std::string port);
		void			setFd(int fd);

	private:
		std::string 			_host;
		std::string 			_port;
		int						_fd;
};

#endif