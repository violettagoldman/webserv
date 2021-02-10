/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablanar <ablanar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/10 15:18:22 by ablanar           #+#    #+#             */
/*   Updated: 2021/02/10 15:40:38 by ablanar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.class.hpp"
#include "../inc/Config.class.hpp"
#include "../inc/Location.class.hpp"
#include "../inc/VirtualHost.class.hpp"
#include <sys/time.h>
#include <unistd.h>
#include "../inc/Request.class.hpp"

int check_listen(VirtualHost host, Request *req)
{
	(void)req;
	(void)host;
	return 1;
}

 void handler(Request *req, Config conf)
 {

	 std::vector<VirtualHost> hosts;
	 hosts = conf.getVirtualHostVector();
	 for (std::vector<VirtualHost>::iterator it = hosts.begin(); it <  hosts.end(); ++it)
	 {
		 if (check_listen(*it, req))
		 	std::cout << "kek";
	 }
 }
