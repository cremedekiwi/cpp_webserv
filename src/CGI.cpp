/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 10:17:45 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/27 12:59:10 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "Exceptions.hpp"
#include "Server.hpp"
#include "color.hpp"
#include "webserv.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>

// write the request body to pipe redirected to in CGI's stdin
// redirect the CGI's stdout to the connection_fd
// prepare argv and envp for execve
// calls execve with the right exectuable
// after fork() error must be sent from the fork
pid_t CGI::makeCGI(const Request &req, const int connection_fd, const Server &server)
{
	(void)connection_fd;

	std::vector<char *> envp_vec;
	char				buffer[1024];
	std::string			errorPage = server.getErrorPage(500);
	const char		   *args[3] = {req.getCGIExec().c_str(), realpath(req.getServerPath().c_str(), buffer), NULL};

	pid_t f = fork();
	if (f < 0)
		throw(ServerErrorException("fork failed", 500));
	if (f != 0)
	{
		close(req.getCgiPIn()[0]);
		close(req.getCgiPOut()[1]);
		return f;
	}

	envp_vec = buildEnv(req, server.getEnvp(), server.getConfig("server_name"), server.getSockaddrs());

	if (dup2(req.getCgiPIn()[0], STDIN_FILENO) < 0)
		exit(1);
	if (dup2(req.getCgiPOut()[1], STDOUT_FILENO) < 0)
		exit(1);
	if (chdir(req.getCGIFolder().c_str()))
		exit(1);
	close(req.getCgiPIn()[0]);
	close(req.getCgiPOut()[0]);
	close(req.getCgiPOut()[1]);

	for (int i = 3; i <= server_fd_max; i++)
		close(i);

	execve(req.getCGIExec().c_str(), (char **)args, envp_vec.data());
	for (std::vector<char *>::iterator it = envp_vec.begin(); it != envp_vec.end(); ++it)
		free(*it);
	exit(1);
}

std::vector<char *> CGI::buildEnv(const Request &req, char **envp, const std::string &server_name,
								  const sockaddr_in &sockaddrs)
{
	std::vector<char *> env;

	for (int i = 0; envp[i]; i++)
		env.push_back(strdup(envp[i]));

	addEnvKey(env, "CONTENT_LENGTH", req.getHeaderArgument("Content-Length"));
	addEnvKey(env, "CONTENT_TYPE", req.getHeaderArgument("Content-Type"));
	addEnvKey(env, "GATEWAY_INTERFACE", "CGI/1.1");
	addEnvKey(env, "PATH_TRANSLATED", req.getServerPath());
	addEnvKey(env, "PATH_INFO", req.getServerPath());
	addEnvKey(env, "QUERY_STRING", req.getQueryString());
	addEnvKey(env, "REMOTE_ADDR", ft_to_string(ntohs(req.getClientAddr().sin_addr.s_addr)));
	addEnvKey(env, "REMOTE_HOST", ft_to_string(ntohs(req.getClientAddr().sin_addr.s_addr)));
	addEnvKey(env, "REMOTE_PORT", ft_to_string(ntohs(req.getClientAddr().sin_port)));
	addEnvKey(env, "REMOTE_USER", "");
	addEnvKey(env, "REQUEST_METHOD", req.getMethod());
	addEnvKey(env, "SCRIPT_NAME", req.getCGIScript());
	addEnvKey(env, "SERVER_NAME", server_name);
	addEnvKey(env, "SERVER_PORT", ft_to_string(ntohs(sockaddrs.sin_port)));
	addEnvKey(env, "SERVER_PROTOCOL", "HTTP/1.1");
	addEnvKey(env, "SERVER_SOFTWARE", "webserv/1.0");
	env.push_back(NULL);
	return env;
}

void CGI::addEnvKey(std::vector<char *> &env, const std::string key, const std::string value)
{
	env.push_back(strdup((char *)(key + "=" + value).c_str()));
}

void CGI::sendError(std::string &error, int fd)
{
	std::cerr << "CGI error" << std::endl;
	const std::string codestr = "500 Internal Server Error";
	std::string		  str = "HTTP/1.1 " + codestr + " \r\n";
	str += "\r\n";
	if (error.empty())
	{
		str += "<html><head>";
		str += "<meta http-equiv=\"content-type\" content=\"text/html; charset=windows-1252\">";
		str += "<title>";
		str += codestr;
		str += "</title></head>";
		str += "<body>";
		str += "<center><h1>";
		str += codestr;
		str += "</h1></center>";
		str += "<hr><center>werbserv/1.0</center>";
		str += "</body></html>";
	}
	else
		str += error;
	write(fd, str.c_str(), str.size());
	close(fd);
	exit(1);
}

void CGI::displayLog(const std::string &str)
{
	std::cerr << color::green;
	displayTimestamp(std::cerr);
	std::cerr << 200 << " " << str;
	std::cerr << color::reset << std::endl;
}
