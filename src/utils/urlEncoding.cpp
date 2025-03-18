/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urlEncoding.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-tell <ale-tell@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:35:01 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/24 17:35:01 by ale-tell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <map>

strmap codes;

const strmap &getURLCodes()
{
	return codes;
}

void initURLCodes()
{
	codes["%3A"] = ":";
	codes["%2F"] = "/";
	codes["%3F"] = "?";
	codes["%23"] = "#";
	codes["%5B"] = "[";
	codes["%5D"] = "]";
	codes["%40"] = "@";
	codes["%21"] = "!";
	codes["%24"] = "$";
	codes["%26"] = "&";
	codes["%27"] = "'";
	codes["%28"] = "(";
	codes["%29"] = ")";
	codes["%2A"] = "*";
	codes["%2B"] = "+";
	codes["%2C"] = ",";
	codes["%3B"] = ";";
	codes["%3D"] = "=";
	codes["%25"] = "%";
	codes["%20"] = " ";
}
