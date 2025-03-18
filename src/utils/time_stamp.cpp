/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_stamp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:08:17 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/25 14:26:42 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <iomanip>
#include <iostream>

#include "webserv.hpp"

std::ostream &displayTimestamp(std::ostream &os)
{
	std::time_t t = std::time(NULL);
	struct tm  *now = std::localtime(&t);

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);

	os << buffer << " ";
	return os;
}
