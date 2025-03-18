/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SuperServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:00:39 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/27 13:02:12 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SuperServer.hpp"
#include "ConfigParser.hpp"
#include "Exceptions.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "color.hpp"
#include "webserv.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <stdexcept>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>

volatile int server_fd_max = 3;

SuperServer::~SuperServer()
{
	std::map<int, std::vector<Server> >::iterator it;
	for (it = servers.begin(); it != servers.end(); it++)
		close(it->first);

	std::map<int, Request>::iterator itp;
	for (itp = requests.begin(); itp != requests.end(); itp++)
		close(it->first);
}

SuperServer::SuperServer(superServerConfig &config, char **env)
{
	maxFD = 1023;
	envp = env;
	FD_ZERO(&write_fds);
	FD_ZERO(&read_fds);
	superServerConfig::iterator it;
	for (it = config.begin(); it != config.end(); it++)
		addServer(*it);
}

void SuperServer::setnonblock(int sock)
{
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FD_CLOEXEC) < 0)
		throw(std::runtime_error("fcntl set error"));
}

void SuperServer::addServer(serverConfig &config)
{

	Server server(envp, config.first["error"], config);
	int	   fd;

	if (host_port_to_fd.find(config.first["host_port"]) == host_port_to_fd.end())
	{
		fd = server.createSocket();
		server_fd_max++;
		setnonblock(fd);
		host_port_to_fd[config.first["host_port"]] = fd;
		FD_SET(fd, &read_fds);
	}
	else
	{
		std::vector<Server>::iterator it;
		fd = host_port_to_fd[config.first["host_port"]];
		for (it = servers[fd].begin(); it != servers[fd].end(); it++)
		{
			if (it->getConfig("server_name") == config.first["server_name"])
			{
				std::cerr << color::red << "Duplicate server: " << config.first["server_name"] << std::endl;
				return;
			}
		}
	}
	servers[fd].push_back(server);
}

void SuperServer::start()
{
	fd_set		   read_fds_copy;
	fd_set		   write_fds_copy;
	struct timeval timeout;

	while (gSignalStatus != SIGINT)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000;
		FD_ZERO(&read_fds_copy);
		FD_ZERO(&write_fds_copy);
		memcpy(&read_fds_copy, &read_fds, sizeof(fd_set));
		memcpy(&write_fds_copy, &write_fds, sizeof(fd_set));
		if (select(maxFD + 1, &read_fds_copy, &write_fds_copy, NULL, &timeout) < 0)
		{
			if (gSignalStatus == SIGINT)
				break;
			else
				throw(std::runtime_error("select failed"));
		}
		try
		{
			for (int fd = 0; fd < maxFD + 1; fd++)
			{
				// std::cout << fd << std::endl;
				if (FD_ISSET(fd, &read_fds_copy))
				{
					if (cgis.find(fd) != cgis.end())
						read_cgi_output(fd);
					else if (servers.find(fd) != servers.end())
						openConnection(fd);
					else
						readRequest(fd);
				}
				if (FD_ISSET(fd, &write_fds_copy))
				{
					if (cgis.find(fd) != cgis.end())
						write_body_to_cgi(fd);
					else
						sendResponse(fd);
				}
			}
		}
		catch (SuperServer::SuperServerException &e)
		{
			closeConnection(e.getFd());
			if (*e.what() != '\0')
				std::cerr << color::red << e.what() << color::reset << std::endl;
		}
		catch (std::exception &e)
		{
			std::cerr << color::red << e.what() << color::reset << std::endl;
		}
	}
}

void SuperServer::createCGI(const int connection_fd)
{
	Request &req = requests[connection_fd];

	int p_in[2];
	int p_out[2];
	pipe(p_in);
	pipe(p_out);
	setnonblock(p_in[1]);
	setnonblock(p_out[0]);
	FD_SET(p_in[1], &write_fds);
	cgis[p_in[1]] = connection_fd;
	req.getResponse().appendRaw("HTTP/1.1 200 OK\r\n");
	req.setCgiPIn(p_in);
	req.setCgiPOut(p_out);
}

void SuperServer::write_body_to_cgi(const int p_in_fd)
{
	int		 connection_fd = cgis[p_in_fd];
	Request &req = requests[connection_fd];

	ssize_t nWrite = write(p_in_fd, req.getBody().c_str(), req.getBody().size());
	if (nWrite < 0)
		throw(SuperServer::SuperServerException("write failed", connection_fd));
	req.cutbody(nWrite);
	if (req.getBody().size() == 0 || nWrite == 0)
	{
		FD_CLR(p_in_fd, &write_fds);
		close(req.getCgiPIn()[1]);
		FD_SET(req.getCgiPOut()[0], &read_fds);
		cgis[req.getCgiPOut()[0]] = connection_fd;
		cgis.erase(p_in_fd);
		pids[req.getCgiPOut()[0]] = req.getServer()->launchCGI(requests[connection_fd], connection_fd);
	}
}

void SuperServer::read_cgi_output(const int p_out_fd)
{
	int		 connection_fd = cgis[p_out_fd];
	Request &req = requests[connection_fd];
	char	 buffer[READ_SIZE + 1];
	int		 status;
	memset(buffer, 0, READ_SIZE + 1);

	ssize_t nRead = read(p_out_fd, buffer, READ_SIZE);
	if (nRead < 0)
		throw(SuperServer::SuperServerException("read failed", connection_fd));
	req.getResponse().appendRaw(buffer);

	if (waitpid(pids[p_out_fd], &status, WNOHANG))
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			req.getResponse().serializeError(500, req.getMethod() != "HEAD");
		FD_CLR(p_out_fd, &read_fds);
		close(req.getCgiPOut()[0]);
		FD_SET(connection_fd, &write_fds);
		cgis.erase(p_out_fd);
		pids.erase(p_out_fd);
		req.getResponse().setPathCode(std::string(), 200);
	}
}

void SuperServer::openConnection(const int server_fd)
{
	struct sockaddr_in addr_client;
	socklen_t		   len = sizeof(sockaddr_in);

	memset(&addr_client, 0, len);
	int connection_fd = accept(server_fd, (struct sockaddr *)&addr_client, &len);
	if (connection_fd < 0)
		throw(std::runtime_error("accept failed"));
	setnonblock(connection_fd);
	FD_SET(connection_fd, &read_fds);
	requests[connection_fd] = Request(addr_client, server_fd);
}

void SuperServer::readRequest(const int connection_fd)
{
	Request &req = requests[connection_fd];
	char	 buffer[READ_SIZE + 1];
	ssize_t	 nRead;

	memset(buffer, 0, READ_SIZE + 1);
	nRead = recv(connection_fd, buffer, READ_SIZE, MSG_DONTWAIT);
	if (nRead < 0)
		throw(SuperServer::SuperServerException("recv failed", connection_fd));
	if (nRead == 0)
		throw(SuperServer::SuperServerException("", connection_fd));

	bool skip = req.appendRaw(buffer);
	try
	{
		if (!req.getServer())
			attributeServer(req);
		req.checkedHeader();
		if (!skip)
			return;
		size_t bodySize = req.getServer()->parseRequest(req);
		if (req.isChunked() && bodySize != 0)
			return;
		if (!req.getCGIScript().empty())
			createCGI(connection_fd);
	}
	catch (WebservExpection &e)
	{
		std::cout << color::red << "Error " << e.what() << color::reset << std::endl;
		req.getResponse().serializeError(e.getCode(), req.getMethod() != "HEAD");
	}
	catch (std::exception &e)
	{
		std::cout << color::red << "Error " << e.what() << color::reset << std::endl;
		req.getResponse().serializeError(500, req.getMethod() != "HEAD");
	}
	FD_CLR(connection_fd, &read_fds);
	if (req.getCGIScript().empty())
		FD_SET(connection_fd, &write_fds);
}

void SuperServer::attributeServer(Request &req)
{
	std::vector<Server>::iterator it;
	const std::string			 &host_name = req.getHeaderArgument("Host");
	const int					  server_fd = req.getServerFD();

	if (host_name == std::string())
		return req.setServer(&servers[server_fd][0]);

	for (it = servers[server_fd].begin(); it != servers[server_fd].end(); it++)
	{
		if (it->getConfig("name_port") == host_name)
			return req.setServer(&*it);
	}
	req.setServer(&servers[server_fd][0]);
}

void SuperServer::sendResponse(const int connection_fd)
{
	ssize_t	  nWrite;
	Server	 *server = requests[connection_fd].getServer();
	Response &res = requests[connection_fd].getResponse();

	if (res.getRaw().empty())
		server->makeResponse(requests[connection_fd], connection_fd);

	const std::string &response = res.getRaw();

	nWrite = send(connection_fd, response.c_str(), response.size(), MSG_DONTWAIT);
	if (nWrite < 0)
		throw(SuperServer::SuperServerException("send failed", connection_fd));
	else if (nWrite == 0)
		throw(SuperServer::SuperServerException("", connection_fd));
	if ((size_t)nWrite != response.size())
		return res.cutRaw(nWrite);
	res.cutRaw(nWrite);
	std::cout << res << std::endl;
	closeConnection(connection_fd);
}

void SuperServer::closeConnection(const int connection_fd)
{
	FD_CLR(connection_fd, &write_fds);
	FD_CLR(connection_fd, &read_fds);
	requests.erase(connection_fd);
	close(connection_fd);
}

std::ostream &operator<<(std::ostream &os, SuperServer &super)
{
	std::map<int, std::vector<Server> >::iterator it;
	std::vector<Server>::iterator				  it_vec;

	for (it = super.servers.begin(); it != super.servers.end(); it++)
	{
		for (it_vec = it->second.begin(); it_vec != it->second.end(); it_vec++)
			os << *it_vec;
	}
	os << std::endl;
	return os;
}

SuperServer::SuperServerException::SuperServerException(const std::string &msg, short f) : fd(f)
{
	std::string str = (msg + " (closing connection fd: " + ft_to_string(fd) + ")");
	size_t		size = std::min(msg.size(), static_cast<size_t>(MAX_MSG_LEN - 1));
	memcpy(what_msg, msg.c_str(), size);
	what_msg[size] = 0;
}

char const *SuperServer::SuperServerException::what() const throw()
{
	return what_msg;
}

short SuperServer::SuperServerException::getFd() const
{
	return fd;
}
