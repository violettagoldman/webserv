/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 12:48:18 by ashishae          #+#    #+#             */
/*   Updated: 2021/01/27 12:52:27 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../config/get_next_line/get_next_line.hpp"
#include <iostream>

int main(void)
{
	char *line;
	int ret;
	std::cout << "Reader launched" << std::endl;
	while((ret = get_next_line(0, &line)))
	{
		std::cout << "Stdin: " << line << std::endl;
	}
	/* code */
	return 0;
}