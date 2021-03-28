/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:18:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/03/28 15:18:20 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.class.hpp"
#include "Config.class.hpp"
#include "Location.class.hpp"
#include "VirtualHost.class.hpp"
#include <sys/time.h>
#include <unistd.h>
#include "Request.class.hpp"

int check_listen(VirtualHost host, Header host_header)
{
	std::string request_host;
	std::vector<std::string>  server_names = host.getServerName();
	size_t pos;
	int request_server_port;
	std::string request_server_name;


	request_host = host_header.getValue()[0];
	std::cout << host.getListenIp() << std::endl;
	if ((pos = host_header.getValue()[0].find(':')) != std::string::npos)
	{
		request_server_name = host_header.getValue()[0].substr(0, pos);
		request_server_port = std::atoi((host_header.getValue()[0].substr(pos + 1)).c_str());
	}
	else
	{
		request_server_name = host_header.getValue()[0];
		//probably should set to default supported
		request_server_port = host.getListenIp();
	}
	for (std::vector<std::string>::iterator it = server_names.begin(); it < server_names.end(); ++it)
	{
		std::cout << "Server name: " <<*it << std::endl;
		if (*it == "" && host.getListenIp() == request_server_port)
			return 1;
		if (*it == request_server_name &&  host.getListenIp() == request_server_port)
			return 1;
	}
	return 0;
}

std::string path_to_pattern(std::string path)
{
	size_t pos;

	pos = path.find_last_of('/');
	if (pos == 0)
		pos = std::string::npos;
	std::cout << "Patter:" << path.substr(0, pos) << ";" << std::endl;
	return path.substr(0, pos);
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

// std::vector<Location>::iterator check_location(VirtualHost host, std::string request_path)
// {
// 	std::string request_pattern = path_to_pattern(request_path);
// 	std::vector<Location> server_locations = host.getLocations();
// 	int count_max = 0;
// 	int count_cur;
// 	std::vector<Location>::iterator it_best;
//
// 	for (std::vector<Location>::iterator it = server_locations.begin(); it < server_locations.end(); ++it)
// 	{
// 		if ((count_cur = count_match((*it).getPattern(), request_pattern)) > count_max)
// 		{
// 			it_best = it;
// 			count_max = count_cur;
// 		}
// 	}
// 	if (count_max > 0)
// 		return it_best;
// 	return server_locations.end();
// }

std::string create_final_path(Location loc, std::string request_path)
{
	std::string root = loc.getRoot();
	// std::string final(root + request_path.substr(1));
	(void)request_path;
	std::cout << "Root "<< root << std::endl;
	return root;
}

bool LimitExceptCheck(std::vector<std::string> exceptions, std::string request_method)
{
	for (std::vector<std::string>::iterator it = exceptions.begin(); it < exceptions.end(); ++it)
		if (*it == request_method)
			return true;
	return false;
}

std::string handler(Request &req, Config conf)
{
	 std::vector<VirtualHost> hosts(conf.getVirtualHostVector());
	 std::string final("");
	 Header host_header = *(req.getHeaderByName("Host"));

	 for (std::vector<VirtualHost>::iterator it = hosts.begin(); it <  hosts.end(); ++it)
	 {
		 std::cout << "Hae:" << check_listen(*it, host_header) << ";" << std::endl;
		 if (check_listen(*it, host_header))
		 {
			std::string request_path = req.getPath();
			std::string request_pattern = path_to_pattern(request_path);
			std::vector<Location> server_locations = (*it).getLocations();
			int count_max = 0;
			int count_cur;
			std::vector<Location>::iterator it_best;
			for (std::vector<Location>::iterator it_loc = server_locations.begin(); it_loc < server_locations.end(); ++it_loc)
			{

				if (it_loc->getPattern() == request_path)
				{
					it_best = it_loc;
					break;
				}
				if ((count_cur = count_match((*it_loc).getPattern(), request_pattern)) > count_max)
				{
					it_best = it_loc;
					count_max = count_cur;
				}
			}
			if (count_max == 0)
				it_best = server_locations.end();
			if (it_best != server_locations.end())
			{
				if (((*it_best).getLimitExcept()).isEmpty() != true && LimitExceptCheck((*it_best).getLimitExcept().getMethods(), req.getMethod()) == false)
				{
					std::cout << "KEK" << std::endl;
					req.setError(405);
					return final;
				}
				final = create_final_path(*it_best, request_path);
				return final;
			}
			else
			{
				std::cout << "Not found" << std::endl;
				req.setError(404);
				std::cout << "404" << std::endl;
			}
		 }
	 }
	return final;
}

Location	handlerGetLocation(Request req, Config conf)
{
	Header host_header = *(req.getHeaderByName("Host"));
	std::vector<VirtualHost> hosts(conf.getVirtualHostVector());

	std::string request_path = req.getPath();
	std::string request_pattern = path_to_pattern(request_path);
	int count_max = 0;
	int count_cur;
	std::vector<Location>::iterator it_best;
	std::vector<Location> best_loc;
	for (std::vector<VirtualHost>::iterator it = hosts.begin(); it <  hosts.end(); ++it)
	{
		std::vector<Location> server_locations = it->getLocations();
		for (std::vector<Location>::iterator it_loc = server_locations.begin(); it_loc < server_locations.end(); ++it_loc)
		{
			if ((*it_loc).getIndex().size() != 0)
			{
				std::cout << "Pattern " << (*it_loc).getPattern() <<  std::endl;
				std::cout << "Index " << (*it_loc).getIndex()[0] << std::endl;
				std::cout << "no zero" << std::endl;
			}
			if (it_loc->getPattern() == request_path)
				return (*it_loc);
			if ((count_cur = count_match((*it_loc).getPattern(), request_pattern)) > count_max)
			{
				it_best = it_loc;
				best_loc.push_back(*it_loc);
				count_max = count_cur;
				std::cout << "Pattern best" << (*it_best).getPattern() <<  std::endl;
				std::cout << "Index best" << (*it_best).getIndex()[0] << std::endl;
				std::cout << "no zero best" << std::endl;
			}
		}
	}
	// std::cout << "get root " << (best).getPattern() << std::endl;
	// std::cout << "get index " << best_index.size() << std::endl;
	return (best_loc[best_loc.size() - 1]);
}
