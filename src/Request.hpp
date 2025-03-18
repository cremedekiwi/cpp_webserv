/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 08:36:15 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/25 20:25:54 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ConfigParser.hpp"
#include "Response.hpp"
#include "webserv.hpp"
#include <cstddef>
#include <netinet/in.h>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>

class Server;

class Request
{

  public:
	Request() : raw("") {};
	Request(const struct sockaddr_in &client_addr, const int server_fd);
	~Request() {};

	bool							   appendRaw(std::string line);
	bool							   parseHeader();
	void							   checkedHeader();
	size_t							   parseBody(size_t max_body_size);
	std::map<std::string, std::string> parseCookie();
	std::map<std::string, std::string> parseSession();
	void makeServerPath(const std::string &name, const std::string &root, const std::string &cgi);

	const std::string &getMethod() const;
	const std::string &getUriPath() const;
	const std::string &getServerPath() const;
	const std::string &getBody() const;
	const std::string  getCookie(const std::string &name) const;
	const std::string  getHeaderArgument(const std::string &key) const;
	const std::string &getQueryString() const;
	struct sockaddr_in getClientAddr() const;
	const std::string &getPathInfo() const;
	const std::string &getCGIScript() const;
	const std::string &getCGIExec() const;
	int				   getServerFD() const;
	Server			  *getServer() const;
	locationConfig	  *getLocation() const;
	Response		  &getResponse();
	const std::string &getCGIFolder() const;
	size_t			   getContentLength() const;
	int				  *getCgiPOut() const;
	int				  *getCgiPIn() const;
	bool			   isChunked() const;

	void cutbody(const ssize_t size);
	void setServerPath(const std::string &f);
	void setServer(Server *server);
	void setLocation(locationConfig *loc);
	void setResponse(const Response &res);
	void setCgiPOut(const int fd[2]);
	void setCgiPIn(const int fd[2]);

	friend std::ostream &operator<<(std::ostream &os, Request &request);

  private:
	void setChunked();
	bool parseMethod(std::stringstream &ssRaw);
	bool parseURI(std::stringstream &ssRaw);
	bool parseProtocol(std::stringstream &ssRaw);
	bool parseHeaderAgruments(std::stringstream &ssRaw);

	int				   serverFD;
	size_t			   parsed_len;
	Server			  *server;
	locationConfig	  *loc;
	Response		   res;
	struct sockaddr_in client_addr;
	std::string		   raw;
	std::string		   method;
	std::string		   uri;
	std::string		   uriPath;
	std::string		   queryString;
	std::string		   uriLocation;
	std::string		   serverPath;
	std::string		   file;
	std::string		   protocol;
	std::string		   body;
	std::string		   http;
	std::string		   version;
	strmap			   header_arguments;
	std::string		   cgi_script;
	std::string		   cgi_exec;
	std::string		   cgi_folder;
	size_t			   content_length;
	bool			   chunked;
	int				   cgi_p_int[2];
	int				   cgi_p_out[2];
};
