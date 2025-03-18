/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SuperServer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 13:30:23 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/26 15:06:20 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ConfigParser.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include <exception>
#include <netinet/in.h>
#include <sched.h>
#include <sys/select.h>
#include <vector>

class SuperServer
{

  public:
	SuperServer(superServerConfig &config, char **env);
	~SuperServer();

	void start();
	void addServer(serverConfig &config);

	friend std::ostream &operator<<(std::ostream &os, SuperServer &super);
	void				 setnonblock(int sock);
	void				 createCGI(const int connextion_fd);

	class SuperServerException : std::exception
	{
	  public:
		SuperServerException(const std::string &msg, short fd);
		virtual char const *what() const throw();
		short				getFd() const;

	  private:
		char  what_msg[MAX_MSG_LEN];
		short fd;
	};

  private:
	void openConnection(const int fd);
	void readRequest(int fd);
	void sendResponse(int fd);
	void closeConnection(const int fd);
	void attributeServer(Request &req);
	void read_cgi_output(const int fd);
	void write_body_to_cgi(const int fd);

	fd_set								write_fds;
	fd_set								read_fds;
	int									maxFD;
	std::map<int, std::vector<Server> > servers;
	std::map<int, Request>				requests;
	std::map<std::string, int>			host_port_to_fd;
	std::map<int, int>					cgis;
	std::map<int, pid_t>				pids;
	char							  **envp;
};
