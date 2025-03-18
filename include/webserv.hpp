/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 09:32:18 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/24 17:01:53 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <ios>
#define READ_SIZE 65535

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>


typedef std::map<std::string, std::string> strmap;
static const short			MAX_MSG_LEN = 200;

extern volatile sig_atomic_t gSignalStatus;
extern volatile int			 server_fd_max;
extern std::string				 whitespace;

long	 ft_inet_addr(const std::string &ip);
std::ostream &displayTimestamp(std::ostream &os);
void	 handleSignal(int signal);
std::string	 ft_strtrim(const std::string &str);
char   **ft_mtoargv(const char *exec, const strmap map);

std::string getMimeType(const std::string &file);
void   initMimeTypes();

const std::map<std::string, std::string> &getURLCodes();
void									  initURLCodes();

std::string		   getHttpIdentifier(const int code);
void		   initHTTPIdentifiers();
std::vector<std::string> ft_split(const std::string &str, const char delim);
bool		   ft_isnum(const std::string &str);

std::string getExpirationDate(int seconds);

template <typename T>
T ft_stoi(const std::string &str, std::ios_base &(*mani)(std::ios_base &) = std::dec)
{
	T ret;

	std::stringstream stream;
	stream << mani << str;
	stream >> ret;
	return ret;
}

template <typename T>
std::string ft_to_string(T x)
{
	std::stringstream ss;
	std::string		 ret;

	ss << x;
	return ss.str();
}
