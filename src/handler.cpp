/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:18:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/04/14 17:26:26 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.class.hpp"
#include "Config.class.hpp"
#include "Location.class.hpp"
#include "VirtualHost.class.hpp"
#include <sys/time.h>
#include <unistd.h>
#include "Request.class.hpp"
# include <sys/stat.h>
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

	// std::cout << "here" << std::endl;
	pos = path.find_last_of('/');
	if (pos == 0)
		pos = std::string::npos;
	// std::cout << "Patter:" << path.substr(0, pos) << ";" << std::endl;
	return path.substr(0, pos);
}

bool extensionCheck(std::string loc, std::string req)
{
	// struct stat		fileStat;
	// int				fd;
	std::string extension;
	std::string filename;
	size_t pos;
	// std::cout << "LOCATION IN EXTCHECK: "<< loc << std::endl;
	// std::cout << "IN CHECK : " << loc.substr(loc.find("*") + 1)  << std::endl;
	if (loc.find("*") == std::string::npos)
		return 0;
	// fd = open(req.c_str(), O_RDONLY);
	// if (fd < 0)
	// 	close(fd);
	// else
	// {
	// 	fstat(fd, &fileStat);
	// 	if (S_ISDIR(fileStat.st_mode))
	// 	{
	// 		close(fd);
	// 		return 0;
	// 	}
	// }
	pos = req.find_last_of('/');
	if (pos != std::string::npos && req.substr(pos).find_last_of('.') != std::string::npos)
	{
		filename = req.substr(pos);
		extension = filename.substr(filename.find_last_of('.'));
	}
	else if (req.find_last_of('.') != std::string::npos)
		extension = req.substr(req.find_last_of('.'));
	// std::cout << "EXTENSION : " << extension << std::endl;
	if (extension == "")
		return 0;
	std::cout << loc << std::endl;
	if (loc.substr(loc.find("*") + 1) == extension)
	{
		std::cout << "ok " << std::endl;
		return 1;
	}
	return 0;
}

int count_match(std::string str1, std::string str2)
{
	int count = 0;
	size_t i = 0;
	size_t pos;
	std::string extension;
	std::string filename;
	if (extensionCheck(str1, str2))
	{
		pos = str2.find_last_of('/');
		if (pos != std::string::npos && str2.substr(pos).find_last_of('.') != std::string::npos)
		{
			filename = str2.substr(pos + 1);
			extension = filename.substr(0, filename.find_last_of('.'));
			// std::cout << "FILENAME IN COUNTMATCH " << extension << std::endl;
		}
		str1.replace(str1.find("*"), filename.size(), filename);
		// std::cout << "STR1" << str1 <<std::endl;
	}
	else
		str2  = path_to_pattern(str2);
	while (i < str1.size() && i < str2.size())
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
	std::string location_pattern(loc.getPattern());
	std::string request_path_f(request_path);
	struct stat		fileStat;
	int				fd;

	// std::cout << request_path << std::endl;
	// std::cout << root << std::endl;
	// std::cout << location_pattern << std::endl;
	location_pattern = path_to_pattern(location_pattern);
	if (request_path!= "/")
	{
		// if (location_pattern[location_pattern.size() - 1] == '/')
		// 	location_pattern.erase(location_pattern.size() - 1);
		request_path_f.erase(request_path_f.find(location_pattern), location_pattern.length());
	}
	// std::cout << "1 " << std::endl;
	if (root[root.size() - 1] == '/' && request_path_f[0] == '/')
		request_path_f.erase(0, 1);
	std::string final(root + request_path_f);
	fd = open(final.c_str(), O_RDONLY);
	fstat(fd, &fileStat);
	if (S_ISDIR(fileStat.st_mode) && final[final.size() - 1] != '/')
		final += "/";
	close(fd);
	// std::cout << "Final: "<< final << std::endl;
	return final;
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
		 // std::cout << "Hae:" << check_listen(*it, host_header) << ";" << std::endl;
		 if (check_listen(*it, host_header))
		 {
			std::string request_path = req.getPath();
			// std::cout << "REQUEST PATH" << request_path << "\n\n" << std::endl;
			// std::string request_pattern = path_to_pattern(request_path);
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
				if ((count_cur = count_match((*it_loc).getPattern(), request_path)) > count_max)
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
				if ((*it_best).getClientMaxBodySize() > 0 && (*it_best).getClientMaxBodySize() < req.getBodyLength())
				{
					std::cout << (*it_best).getClientMaxBodySize()  << std::endl;
					std::cout << req.getBodyLength() << std::endl;
					req.setError(413);
					return final;
				}
				if ((*it_best).needsAuth() && !(*it_best).authenticate(req))
				{
					req.setError(401);
					std::cout << "Auth" << std::endl;
					return final;
				}
				std::cout << "good " << std::endl;
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
			// if ((*it_loc).getIndex().size() != 0)
			// {
			// 	std::cout << "Pattern " << (*it_loc).getPattern() <<  std::endl;
			// 	std::cout << "Index " << (*it_loc).getIndex()[0] << std::endl;
			// 	std::cout << "no zero" << std::endl;
			// }
			if (it_loc->getPattern() == request_path)
				return (*it_loc);
			if ((count_cur = count_match((*it_loc).getPattern(), request_path)) > count_max)
			{
				it_best = it_loc;
				best_loc.push_back(*it_loc);
				count_max = count_cur;
				// std::cout << "Pattern best" << (*it_best).getPattern() <<  std::endl;
				// std::cout << "Index best" << (*it_best).getIndex()[0] << std::endl;
			}
		}
	}
	// std::cout << "get root " << (best).getPattern() << std::endl;
	// std::cout << "get index " << best_index.size() << std::endl;
	std::cout << "HERE kek" << std::endl;
	return (best_loc[best_loc.size() - 1]);
}
