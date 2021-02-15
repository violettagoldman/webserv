/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:18:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/02/15 21:16:54 by ablanar          ###   ########.fr       */
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

int count_match(std::string str1, std::string str2)
{
	int count = 0;

	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] == str2[i])
			count++;
		i++;
	}
	return count;
}

std::vector<Location>::iterator check_location(VirtualHost host, std::string request_path)
{
	std::string request_pattern = path_to_pattern(request_path);
	std::vector<Location> server_locations = host.getLocations();
	int count_max = 0;
	int count_cur;
	std::vector<Location>::iterator it_best;

	for (std::vector<Location>::iterator it = server_locations.begin(); it < server_locations.end(); ++it)
	{
		if ((count_cur = count_match((*it).getPattern(), request_pattern)) > count_max)
		{
			it_best = it;
			count_max = count_cur;
		}
	}
	if (count_max > 0)
		return it_best;
	return server_locations.end();
}

std::string create_final_path(Location loc, std::string request_path)
{
	std::string root = loc.getRoot();
	std::string final(root + request_path.substr(1));
	return final;
}

std::string handler(Request *req, Config *conf)
 {
	 std::vector<Header *> headers;
	 std::vector<VirtualHost> hosts;
	 Header *host_header;
	std::string final("");
	 hosts = conf->getVirtualHostVector();
	 headers = req->getHeaders();
	 for (std::vector<Header *>::iterator it = headers.begin(); it < headers.end(); ++it)
	 	if ((*it)->getName() == "Host")
			host_header = *it;
	 for (std::vector<VirtualHost>::iterator it = hosts.begin(); it <  hosts.end(); ++it)
	 {
		 if (check_listen(*it, host_header))
		 {
			std::string request_path = req->getPath();
			std::string request_pattern = path_to_pattern(request_path);
			std::vector<Location> server_locations = (*it).getLocations();
			std::vector<Location>::iterator it_best = check_location(*it, request_path);
			if (it_best != server_locations.end())
			{
			 	final = create_final_path(*it_best, request_path);
				return final;
			}
			else
				std::cout << "404" << std::endl;
			 // for (std::vector<Location>::iterator it_loc = server_locations.begin(); it_loc < server_locations.end(); ++it_loc)
			 	// if (check_location())
				 // if ((*it_loc).getPattern() == request_pattern)
				 // 	std::cout << "Kek" << std::endl;
			 // if (check_location(*it, path))
			 	// create_final_path(*it)
		 }
	}
	return final;
 }
