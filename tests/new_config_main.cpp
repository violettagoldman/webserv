/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_config_main.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 12:09:50 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/11 12:17:36 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AConfigReader.class.hpp"

int main(void)
{
	AConfigReader("config/test_configs/nginx.conf");
	return 0;
}