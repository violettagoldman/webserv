#include <sys/time.h>
#include <unistd.h>
#include "Request.class.hpp"
#include "Config.class.hpp"
#include "ConfigReader.class.hpp"
#include "Server.class.hpp"
#include "Response.class.hpp"

Request		*read_request(int sd, Request *req);
std::string handler(Request req, Config conf);

int		main(void)
{
	size_t			i;
	Server			s;
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

	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(s.getFd(), &fds);
		max_sd = s.getFd();
		for (i = 0; i < s.getClients().size(); i++)
		{
			//socket descriptor
			sd = s.getClients()[i];
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &fds);
			// highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
		// wait for an activity on one of the sockets
		select(max_sd + 1 , &fds , NULL , NULL , NULL);
		if (FD_ISSET(s.getFd(), &fds))
		{
			new_socket = s.accept();
			// for (i = 0; i < s.getClients().size(); i++)
			// {
			// 	if(s.getClients()[i] == 0)
			// 	{
			// 		s.addClient(new_socket);
			// 		break;
			// 	}
			// }
			s.addClient(new_socket);
		}
		//else its some IO operation on some other socket
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
					// Response response = Response(*request, conf);
					// s.send(sd, response.serialize());
				}
				else if (request.getState() == "error")
				{
					std::cout << "Error";
				}

			}
		}
	}
	// close ports
	return (0);
}
