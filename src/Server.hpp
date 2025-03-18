/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:57:12 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/26 15:47:03 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ConfigParser.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "SessionManager.hpp"
#include "webserv.hpp"
#include <cstddef>
#include <netinet/in.h>
#include <set>
#include <string>
#include <sys/poll.h>
#include <vector>

class Server
{
  public:
	Server();
	Server(char **env, const std::string errors, serverConfig &config);
	Server(const Server &ref);
	~Server();
	Server						 &operator=(const Server &);
	void						  makeResponse(Request &req, const int connection_fd);
	int							  createSocket();
	size_t						  parseRequest(Request &req);
	const std::string			 &getConfig(const std::string &key);
	std::map<short, std::string> *getErrorPages();
	void						  setErrorPages(const std::string &errors);
	friend std::ostream			 &operator<<(std::ostream &os, Server &server);

	std::string		   getErrorPage(short code) const;
	char			 **getEnvp() const;
	const std::string &getConfig(const std::string &key) const;
	const sockaddr_in &getSockaddrs() const;
	locationConfig	  *findLocation(Request &req);
	pid_t			   launchCGI(Request &req, const int connection_fd);

	static const std::set<std::string> serverValidKeys;
	static const std::set<std::string> serverRequiredKeys;
	static const std::set<std::string> locationValidKeys;
	static const std::set<std::string> locationRequiredKeys;
	static const std::set<std::string> httpMethods;

  private:
	void			del(Request &req, Response &res);
	void			get(Request &req, Response &res);
	void			put(Request &req, Response &res);
	void			handleCookieRequest(Request &req, Response &res);
	void			handleSessionRequest(Request &req, Response &res);
	friend void		printValue(std::ostream &os, std::vector<strmap>::iterator &it, const std::string &key);

	std::map<short, std::string> errorPages;
	strmap						 config;
	std::vector<strmap>			 locations;
	int							 backLog;
	sockaddr_in					 sockaddrs;
	int							 server_fd;
	char					   **envp;
	SessionManager				 sessionManager;
};
