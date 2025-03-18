/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-tell <ale-tell@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:40:20 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/13 22:40:20 by ale-tell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> ft_split(const std::string &str, const char delim)
{
	std::vector<std::string> ret;
	std::string		   s;
	std::stringstream   ss(str);
	while (getline(ss, s, delim))
		ret.push_back(s);
	return ret;
}
