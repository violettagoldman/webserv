#include <sys/time.h>
#include <unistd.h>
#include "Request.class.hpp"
#include "Config.class.hpp"
#include "ConfigReader.class.hpp"
#include "Server.class.hpp"
#include "Response.class.hpp"

Request		*read_request(int sd, Request *req);
std::string handler(Request req, Config conf);

Server	s;

void	shutdown(int signal)
{
	(void)signal;
	std::cout << "Shutting down the server\n";
	s.close();
	exit(0);
}

int		main(void)
{
	size_t			i;
	fd_set			fds;
	int				new_socket;
	Request			request;
	ConfigReader	reader("./tests/config/test_configs/nginx.conf");
	Config 			conf = reader.getConfig();
	int 			sd;
	int 			max_sd;
	std::string 	final_path;

	s = Server();
	s.setup();
	s.listen();

	signal(SIGINT, shutdown);
	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(s.getFd(), &fds);
		max_sd = s.getFd();
		for (i = 0; i < s.getClients().size(); i++)
		{
			sd = s.getClients()[i];
			if(sd > 0)
				FD_SET( sd , &fds);
			if(sd > max_sd)
				max_sd = sd;
		}
		select(max_sd + 1 , &fds , NULL , NULL , NULL);
		if (FD_ISSET(s.getFd(), &fds))
		{
			new_socket = s.accept();
			s.addClient(new_socket);
		}
		for (i = 0; i < s.getClients().size(); i++)
		{
			sd = s.getClients()[i];
			if (FD_ISSET(sd , &fds))
			{
				request.read_request(sd);
				if (request.getState() == "end")
				{
					s.close();
					close(sd);
					s.getClients()[i] = 0;
				}
				else if (request.getState() == "read")
				{
					(void)conf;
					request.print_headers();
					final_path = handler(request, conf);
					Response response = Response(request);
					s.send(sd, response.serialize());
				}
				else if (request.getState() == "error")
				{
					std::cout << "Error";
				}
			}
		}
	}
	return (0);
}
