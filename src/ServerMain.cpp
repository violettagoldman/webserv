#include <sys/time.h>
#include <unistd.h>
#include "ConfigReader.class.hpp"
#include "Server.class.hpp"
#include "Response.class.hpp"
#include "Handler.hpp"
#include "CGIHandler.class.hpp"

Request *read_request(int sd, Request *req);

std::vector<Server> servers;

void shutdown(int signal)
{
	(void)signal;
	std::cout << "Shutting down the server\n";
	for (size_t i = 0; i < servers.size(); ++i)
		servers[i].close();
	exit(0);
}

int main(int argc, char **argv)
{
	size_t i;
	fd_set fds;
	fd_set fds_read;
	fd_set fds_write;
	int new_socket;
	int sd;
	std::string final_path;
	std::string path_to_conf("./tests/config/test_configs/nginx.conf");
	std::map<int, Request> chunked_requests;

	std::map<int, Request>::iterator chunked_pos;
	std::map<int, Response> resp;

	std::map<int, Response>::iterator resp_pos;
	std::vector<Response> response_vec;
	if (argc == 2)
	{
		path_to_conf = argv[1];
	}
	std::cout << "Using config at " << path_to_conf << std::endl;

	Config conf;
	try
	{
		ConfigReader reader(path_to_conf);
		conf = reader.getConfig();
	}
	catch (Exception &e)
	{
		std::cout << "Got a Config exception: " << e.what() << std::endl;
		exit(1);
	}

	for (size_t i = 0; i < conf.getVirtualHostVector().size(); ++i)
	{
		Server server;
		server.setup(conf.getVirtualHostVector()[i]);
		server.listen();
		servers.push_back(server);
	}

	signal(SIGINT, shutdown);

	FD_ZERO(&fds);
	for (size_t i = 0; i < servers.size(); ++i)
	{
		FD_SET(servers[i].getFd(), &fds);
	}
	while (1)
	{
		fds_read = fds;
		fds_write = fds;
		select(FD_SETSIZE, &fds_read, &fds_write, NULL, NULL);

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
				if (FD_ISSET(sd, &fds_read))
				{
					Request request;
					if ((chunked_pos = chunked_requests.find(sd)) != chunked_requests.end())
						request = chunked_pos->second;
					request.read_request(sd);
					if (request.getState() == "processing")
						chunked_requests[sd] = request;
					if ((request.getState() == "read" || request.getState() == "end") && (chunked_pos = chunked_requests.find(sd)) != chunked_requests.end())
						chunked_requests.erase(sd);
					if (request.getState() == "end")
					{
						servers[s].removeClient(sd);
						FD_CLR(sd, &fds);
						close(sd);
					}
					else if (request.getState() == "read" || request.getState() == "error")
					{
						final_path = handler(request, conf);
						Location loc = handlerGetLocation(request, conf); // use all virtual hosts
						if (loc.getFcgiPass() != "" && loc.getCgiExtension() == request.getExtension())
						{
							CGIRequires cr =
									{
											final_path,
											(*(request.getHeaderByName("Host"))).getValue()[0],
											"http://" + (*(request.getHeaderByName("Host"))).getValue()[0] + request.getPath(),
											servers[s].getPort(),
											conf.getVirtualHostVector()[s].getServerName()[0],
											loc.getFcgiPass()};

							CGIHandler handler;
							bool cgiFailed = false;
							try
							{
								handler = CGIHandler(request, cr);
							}
							catch (Exception &e)
							{
								std::cout << "[CGI Exception]: " << e.what() << std::endl;
								cgiFailed = true;
							}

							Response response(handler.getCgiResponse(), cgiFailed);
							response.serialize();
							resp[sd] = response;
						}
						else
						{
							Response response(request, loc, final_path);
							response.serialize();
							resp[sd] = response;
						}
					}
				}
				if (FD_ISSET(sd, &fds_write))
				{
					if ((resp_pos = resp.find(sd)) != resp.end())
					{
						if (servers[s].send(sd, resp_pos->second.getResult(), &resp_pos->second))
							resp.erase(sd);
					}
				}
			}
		}
	}

	return (0);
}
