/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 12:15:59 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/31 13:30:44 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include "femtotest.hpp"


int main(void)
{
	out("Simple CGI");
	CGIHandler h("/Users/ae/spacial-steel/cdng/webserv/cgi/tiny.php", "");

	std::vector<std::string> resp = h.getCgiResponse();

	std::cout << "Response: " << std::endl;

	for (int i = 0; i < resp.size(); i++)
	{
		std::cout << resp[i] << std::endl;
	}
}