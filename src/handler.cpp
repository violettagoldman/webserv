/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:18:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/02/13 15:48:04 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.class.hpp"
#include "../inc/Config.class.hpp"
#include "../inc/Location.class.hpp"
#include "../inc/VirtualHost.class.hpp"
#include <sys/time.h>
#include <unistd.h>
#include "../inc/Request.class.hpp"




int check_listen(VirtualHost host, Header *host_header)
{
	std::string request_host;
	std::vector<std::string>  server_names = host.getServerName();
	size_t pos;
	int request_server_port;
	std::string request_server_name;

	request_host = host_header->getValue()[0];
	std::cout << host.getListenIp() << std::endl;
	if ((pos = host_header->getValue()[0].find(':')) != std::string::npos)
	{
		request_server_name = host_header->getValue()[0].substr(0, pos);
		request_server_port = std::atoi((host_header->getValue()[0].substr(pos + 1)).c_str());
	}
	else
	{
		request_server_name = host_header->getValue()[0];
		//probably should set to default supported
		request_server_port = 8880;
	}
	for (std::vector<std::string>::iterator it = server_names.begin(); it < server_names.end(); ++it)
		if (*it == request_server_name &&  host.getListenIp() == request_server_port)
			return 1;
	return 0;
}

std::string path_to_pattern(std::string path)
{
	size_t pos;

	pos = path.find_last_of('/');
	return path.substr(0, pos + 1);
}

int check_location(VirtualHost host, std::string request_path)
{
	std::vector<Location> server_locations = host.getLocations();
	std::string request_pattern = path_to_pattern(request_path);

	for (std::vector<Location>::iterator it = server_locations.begin(); it < server_locations.end(); ++it)
	{
		std::cout << "Pattern :" << (*it).getPattern() << std::endl;
		std::cout << "Request pattern :" << request_pattern << std::endl;
		if ((*it).getPattern() == request_pattern)
			return 1;
	}
	return 0;
}

void handler(Request *req, Config *conf)
 {
	 std::vector<Header *> headers;
	 std::vector<VirtualHost> hosts;
	 Header *host_header;
	 hosts = conf->getVirtualHostVector();
	 headers = req->getHeaders();
	 for (std::vector<Header *>::iterator it = headers.begin(); it < headers.end(); ++it)
	 	if ((*it)->getName() == "Host")
			host_header = *it;
	 for (std::vector<VirtualHost>::iterator it = hosts.begin(); it <  hosts.end(); ++it)
	 {
		 if (check_listen(*it, host_header))
		 {
			 std::string path = req->getPath();
			 if (check_location(*it, path))
			 	std::cout << "Need to create path pattern+root" << std::endl;
		 }
	 }
 }
