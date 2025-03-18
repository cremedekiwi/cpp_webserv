/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 10:17:46 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/24 12:15:39 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include <netinet/in.h>
#include <string>
#include <vector>

class CGI
{
  public:
	static pid_t makeCGI(const Request &req, const int connection_sfd, const Server &server);

  private:
	static void				   displayLog(const std::string &str);
	static void				   addEnvKey(std::vector<char *> &env, const std::string key, const std::string value);
	static void				   sendError(std::string &error, int fd);
	static std::vector<char *> buildEnv(const Request &req, char **envp, const std::string &server_name,
										const sockaddr_in &sockaddrs);

	~CGI() {}

	CGI() {}
};
