#include <sys/time.h>
#include <unistd.h>
#include "ConfigReader.class.hpp"
#include "Server.class.hpp"
#include "Response.class.hpp"
#include "Handler.hpp"

Request		*read_request(int sd, Request *req);

Server	s;

void	shutdown(int signal)
{
	(void)signal;
	std::cout << "Shutting down the server\n";
	s.close();
	exit(0);
}

int		main(int argc, char **argv)
{
	size_t		i;
	fd_set		fds;
	fd_set		fds_read;
	int			new_socket;
	int 		sd;
	int 		max_sd;
	std::string	final_path;
	std::string	path_to_conf("./tests/config/test_configs/nginx.conf");
	
	if (argc == 2)
	{
		path_to_conf = argv[1];
	}
	std::cout << "Using config at " << path_to_conf << std::endl;
	ConfigReader reader(path_to_conf);

	Config conf = reader.getConfig();

	s = Server();
	s.setup(conf.getVirtualHostVector()[0]); // loop
	s.listen();

	signal(SIGINT, shutdown);

	FD_ZERO(&fds);
	FD_SET(s.getFd(), &fds);
	while(1)
	{
		max_sd = s.getFd();
		for (i = 0; i < s.getClients().size(); i++)
		{
			sd = s.getClients()[i];
			if(sd > 0)
				FD_SET( sd , &fds);
			if(sd > max_sd)
				max_sd = sd;
		}
		fds_read = fds;
		std::cout << "1\n";
		select(max_sd + 1 , &fds_read , NULL , NULL , NULL);
		std::cout << "2\n";
		if (FD_ISSET(s.getFd(), &fds_read))
		{
			new_socket = s.accept();
			s.addClient(new_socket);
			FD_SET(new_socket, &fds);
		}
		for (i = 0; i < s.getClients().size(); i++)
		{
			sd = s.getClients()[i];
			if (FD_ISSET(sd , &fds_read))
			{
				std::cout << "3\n";
				Request request;
				request.read_request(sd);
				std::cout << "4\n";
				std::cout << "RESPONSE STATE " << request.getState() << std::endl;
				if (request.getState() == "end" || request.getState() == "chill")
				{
					// s.close();
					s.removeClient(sd);
					FD_CLR(sd, &fds);
					close(sd);
				}
				else if (request.getState() == "read")
				{
					std::cout << "5\n";
					request.print_headers();
					final_path = handler(request, conf, conf.getVirtualHostVector()[0]);
					Location loc = handlerGetLocation(request, conf.getVirtualHostVector()[0]); // loop
					Response response(request, loc, final_path);
					s.send(sd, response.serialize());
				}
				else if (request.getState() == "error")
				{
					std::cout << "ERROR: " << request.getError() << std::endl;
					s.removeClient(sd);
					FD_CLR(sd, &fds);
					close(sd);
				}
			}
		}
	}
	return (0);
}
