#include <sys/time.h>
#include <unistd.h>
#include "ConfigReader.class.hpp"
#include "Server.class.hpp"
#include "Response.class.hpp"
#include "Handler.hpp"
#include "CGIHandler.class.hpp"

Request		*read_request(int sd, Request *req);

std::vector<Server> servers;

void	shutdown(int signal)
{
	(void)signal;
	std::cout << "Shutting down the server\n";
	for (size_t i = 0; i < servers.size(); ++i)
		servers[i].close();
	exit(0);
}

int		main(int argc, char **argv)
{
	size_t		i;
	fd_set		fds;
	fd_set		fds_read;
	int			new_socket;
	int 		sd;
	std::string	final_path;
	std::string	path_to_conf("./tests/config/test_configs/nginx.conf");
	std::map<int, Request> chunked_requests;
	std::map<int, Request>::iterator chunked_pos;

	if (argc == 2)
	{
		path_to_conf = argv[1];
	}
	std::cout << "Using config at " << path_to_conf << std::endl;
	ConfigReader reader(path_to_conf);
	Config conf = reader.getConfig();
	for (size_t i = 0; i < conf.getVirtualHostVector().size(); ++i)
	{
		Server server;
		server.setup(conf.getVirtualHostVector()[i]);
		server.listen();
		servers.push_back(server);
	}

	signal(SIGINT, shutdown);

	FD_ZERO(&fds);
	for (size_t i = 0; i < servers.size(); ++i) {
		FD_SET(servers[i].getFd(), &fds);
	}
	while(1)
	{
		fds_read = fds;
		select(FD_SETSIZE, &fds_read , NULL , NULL , NULL);

		for (size_t s = 0; s < servers.size(); ++s)
		{
			int server_socket = servers[s].getFd();
			if (FD_ISSET(server_socket, &fds_read))
			{
				new_socket = servers[s].accept();
				servers[s].addClient(new_socket);
				FD_SET(new_socket, &fds);
			}
			for (i = 0; i < servers[s].getClients().size(); i++)
			{
				sd = servers[s].getClients()[i];
				if (FD_ISSET(sd , &fds_read))
				{
					Request request;
					if ((chunked_pos = chunked_requests.find(sd)) != chunked_requests.end())
					{
						std::cout << "kek" << std::endl;
						request = chunked_pos->second;
						request.print_headers();
						// request.setState("chunked");
						std::cout << request.getState() << std::endl;
					}
					request.read_request(sd);

					if (request.getState() == "chunked")
						chunked_requests[sd] = request;
					if (request.getState() == "read" && request.isHeaderPresent("Transfer-Encoding", "chunked"))
					{
							chunked_requests.erase(sd);
							// servers[s].removeClient(sd);
							// FD_CLR(sd, &fds);
							// close(sd);
						}
					if (request.getState() == "end")
					{
						servers[s].removeClient(sd);
						FD_CLR(sd, &fds);
						close(sd);
					}
					else if (request.getState() == "read" || request.getState() == "error")
					{
						std::cout << "Here is ok" << std::endl;
						request.print_headers();
						final_path = handler(request, conf);
						Location loc = handlerGetLocation(request, conf); // use all virtual hosts
						if (loc.getFcgiPass() != "")
						{
							CGIRequires cr =
							{
								final_path,
								 (*(request.getHeaderByName("Host"))).getValue()[0],
								request.getPath(),
								servers[s].getPort(),
								conf.getVirtualHostVector()[s].getServerName()[0],
								loc.getFcgiPass()
							};
							CGIHandler handler(request, cr);
							std::cout << "cgi rep:" << handler.getCgiResponse() << std::endl;
							Response response(handler.getCgiResponse());
							servers[s].send(sd, response.serialize());
						}
						else if (request.getState() != "chunked")
						{
							Response response(request, loc, final_path);
							servers[s].send(sd, response.serialize());
						}
					}
				}
			}
		}
	}

	return (0);
}
