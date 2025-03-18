/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:03:29 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/20 22:20:48 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"
#include <algorithm>
#include <cstddef>
#include <cstring>

WebservExpection::WebservExpection(const std::string &msg, short code) : code(code)
{
	std::string str = (msg + " (HTTP: " + ft_to_string(code) + ")");
	size_t size = std::min(msg.size(), static_cast<size_t>(MAX_MSG_LEN - 1));
	memcpy(what_msg, msg.c_str(), size);
	what_msg[size] = '\0';
}

char const *WebservExpection::what() const throw()
{
	return what_msg;
}

short WebservExpection::getCode() const
{
	return code;
}
