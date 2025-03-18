/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnum.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-tell <ale-tell@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:06:07 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/14 13:06:07 by ale-tell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

bool ft_isnum(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (!isdigit(str[i]))
			return false;
	return true;
}
