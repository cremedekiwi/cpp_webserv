/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_inet_addr.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 09:46:25 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/13 17:26:54 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <iostream>
#include <sstream>
#include <string>

long ft_inet_addr(const std::string &ip)
{
	long ret = 0;

	std::stringstream ssip(ip);
	std::string		  num;
	for (int i = 0; i < 4; i++)
	{
		// try to extract an octet using '.' as a del
		if (!std::getline(ssip, num, '.'))
			throw std::runtime_error("invalid IP address: missing octets");

		// ensure each char in the octet is a digit
		for (std::string::size_type j = 0; j < num.size(); ++j)
		{
			if (!std::isdigit(num[j]))
				throw std::runtime_error("invalid IP address: not a number");
		}
		int byte = ft_stoi<int>(num);
		if (byte < 0 || byte > 255)
			throw std::runtime_error("invalid IP address: octet out of range (0-255)");
	
		// accumulate the result by shifting the octet to its proper position
		ret += byte * (1 << (8 * i));
	}
	// ensure there are no extra octets
	if (std::getline(ssip, num))
		throw std::runtime_error("invalid IP address: too many octets");

	return ret;
}
