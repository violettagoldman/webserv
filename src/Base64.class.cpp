/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base64.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 20:21:51 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/07 15:10:56 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base64.class.hpp"

const std::string sequence = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

Base64::Base64(std::string input) :
	_content(input)
{
}

unsigned char lookup(char c)
{
	unsigned char i = 0;
	while (i < sequence.size())
	{
		if (sequence.at(i) == c)
			return i;
		i++;
	}
	throw Exception("Wrong characters in Base64.");
}

std::string Base64::decode(void)
{
	std::string ret;

	unsigned char* p = (unsigned char*)_content.c_str();

	size_t len = _content.size();

	int pad = len > 0 && (len % 4 || p[len - 1] == '=');
	const size_t L = ((len + 3) / 4 - pad) * 4;

	for (size_t i = 0; i < L; i += 4)
	{
		int n = lookup(p[i]) << 18 | lookup(p[i + 1]) << 12 | lookup(p[i + 2]) << 6 | lookup(p[i + 3]);
		ret.push_back((char)(n >> 16));
		ret.push_back((char)(n >> 8 & 0xFF));
		ret.push_back((char)(n & 0xFF));
	}
	if (pad)
	{
		int n = lookup(p[L]) << 18 | lookup(p[L + 1]) << 12;
		ret.push_back(n >> 16);

		if (len > L + 2 && p[L + 2] != '=')
		{
			n |= lookup(p[L + 2]) << 6;
			ret.push_back(n >> 8 & 0xFF);
		}
	}

	return ret;
}