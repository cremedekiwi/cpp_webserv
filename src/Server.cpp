/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:57:50 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/27 13:01:28 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "CGI.hpp"
#include "Exceptions.hpp"
#include "Request.hpp"
#include "color.hpp"
#include "webserv.hpp"

#include <iomanip>
#include <limits>
#include <sys/stat.h>

const std::string			sv[] = {"host", "port", "max_body_size", "error", "server_name"};
const std::set<std::string> Server::serverValidKeys(sv, sv + 5);
const std::set<std::string> Server::serverRequiredKeys(sv, sv + 3);

const std::string			lv[] = {"name",	   "root", "index",			"types",  "redirection",
									"listing", "cgi",  "max_body_size", "cookie", "session"};
const std::set<std::string> Server::locationValidKeys(lv, lv + 10);
const std::set<std::string> Server::locationRequiredKeys(lv, lv + 1);

const std::string			met[] = {"DELETE", "GET", "POST", "PUT", "HEAD"};
const std::set<std::string> Server::httpMethods(met, met + 5);

Server::Server() {}

Server::~Server() {}

Server::Server(char **env, const std::string errors, serverConfig &c)
{
	envp = env;
	backLog = std::numeric_limits<short>::max();
	memset(&sockaddrs, 0, sizeof(sockaddr_in));
	config = c.first;
	sockaddrs.sin_family = AF_INET;
	sockaddrs.sin_port = htons(ft_stoi<short>(config["port"]));
	sockaddrs.sin_addr.s_addr = ft_inet_addr(config["host"]);
	setErrorPages(errors);
	locations = c.second;
}

Server::Server(const Server &ref)
{
	*this = ref;
}

void Server::setErrorPages(const std::string &errors)
{
	std::string		  code;
	std::string		  fileName;
	std::stringstream ss(errors);

	while (std::getline(ss, code, ' '))
	{
		if (!std::getline(ss, fileName, ' ') || !ft_isnum(code))
			throw(std::runtime_error("invalid config error code: " + code));

		std::ifstream sfile(fileName.c_str());
		if (!sfile.is_open())
			throw(std::runtime_error("invalid config error file: " + fileName));
		std::getline(sfile, errorPages[ft_stoi<short>(code)], '\0');
	}
}

Server &Server::operator=(const Server &ref)
{
	envp = ref.envp;
	config = ref.config;
	backLog = ref.backLog;
	locations = ref.locations;
	sockaddrs = ref.sockaddrs;
	server_fd = ref.server_fd;
	errorPages = ref.errorPages;
	return *this;
}

int Server::createSocket()
{
	int reuseaddr = 1;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		throw(std::runtime_error("socket opening failed"));
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));
	if (bind(server_fd, (struct sockaddr *)&sockaddrs, sizeof(sockaddr)) < 0)
		throw(std::runtime_error("binding failed"));
	listen(server_fd, backLog);
	return server_fd;
}

size_t Server::parseRequest(Request &req)
{
	Response	   &res = req.getResponse();
	locationConfig *loc = req.getLocation();

	try
	{
		req.parseBody(ft_stoi<size_t>(loc->at("max_body_size")));

		std::cout << req;
		if (req.isChunked())
			std::cout << " *CHUNK*";
		std::cout << std::endl;

		return req.getContentLength();
	}
	catch (WebservExpection &e)
	{
		std::cout << color::red << "Error " << e.what() << color::reset << std::endl;
		res.serializeError(e.getCode(), req.getMethod() != "HEAD");
	}
	catch (std::exception &e)
	{
		std::cout << color::red << "other error " << e.what() << color::reset << std::endl;
		res.serializeError(500, req.getMethod() != "HEAD");
	}
	return 0;
}

locationConfig *Server::findLocation(Request &req)
{
	std::vector<locationConfig>::iterator it;

	for (it = locations.begin(); it != locations.end(); it++)
	{
		if (req.getUriPath().find(it->at("name")) == 0)
		{
			if (Server::httpMethods.find(req.getMethod()) == Server::httpMethods.end())
				throw(ServerErrorException("Unsuported method: " + req.getMethod(), 501));
			if (it->at("types").find(req.getMethod()) == it->at("types").npos)
				throw(ClientErrorException("Unauthorized request method on location", 405));
			req.setLocation(&*it);
			if (it->at("redirection").empty())
				req.makeServerPath(it->at("name"), it->at("root"), it->at("cgi"));
			return &*it;
		}
	}
	throw(ClientErrorException("not found", 404));
}

pid_t Server::launchCGI(Request &req, const int connection_fd)
{
	return CGI::makeCGI(req, connection_fd, *this);
}

void Server::makeResponse(Request &req, const int connection_fd)
{
	(void)connection_fd;

	Response		 &res = req.getResponse();
	locationConfig	 *loc = req.getLocation();
	const std::string methode = req.getMethod();

	try
	{
		if (!res.getRaw().empty())
			return;
		else if (!loc->at("redirection").empty())
			res.makeRedirection(loc->at("redirection"), 307);
		else if (methode == "GET" || methode == "HEAD" || methode == "POST")
			get(req, res);
		else if (methode == "PUT")
			put(req, res);
		else if (methode == "DELETE")
			del(req, res);
		res.serializeResponse(methode != "HEAD");
	}
	catch (WebservExpection &e)
	{
		std::cout << color::red << "Error " << e.what() << color::reset << std::endl;
		res.serializeError(e.getCode(), req.getMethod() != "HEAD");
	}
	catch (std::exception &e)
	{
		std::cout << color::red << "other error " << e.what() << color::reset << std::endl;
		res.serializeError(500, req.getMethod() != "HEAD");
	}
}

void Server::handleCookieRequest(Request &req, Response &res)
{
	std::map<std::string, std::string> cookies = req.parseCookie();

	std::string cookieValue = cookies["name"];
	Cookie		cookie("name", cookieValue);
	cookie.setHttpOnly(false);
	cookie.setPath("/");
	cookie.setExpires(getExpirationDate(3600));

	res.addCookie(cookie);
	res.setPathCode(std::string(), 200);
}

void Server::handleSessionRequest(Request &req, Response &res)
{
	// get the current sessionID from cookie
	std::string currentSessionID = req.getCookie("sessionID");
	std::string sessionID;

	if (!currentSessionID.empty() && sessionManager.validateSession(currentSessionID))
		sessionID = currentSessionID; // session is valid, reuse it
	else
		sessionID = sessionManager.createSession(3600); // no session exists, create a new one

	Cookie cookie("sessionID", sessionID);
	cookie.setPath("/");
	cookie.setHttpOnly(false);

	res.addCookie(cookie);
	res.setPathCode(std::string(), 200);
}

/******************************************************************************
find the file to serve to the client:
check the requested path:
	- if it is folder is requested
		- if the uri point to file -> redirection to proper endpoint
		- an index is provided
		- listing is not off
		- index.html is sent
	- if its is a file, it is sent
*******************************************************************************/
void Server::get(Request &req, Response &res)
{
	locationConfig	  *loc = req.getLocation();
	struct stat		   st;
	const std::string &serverPath = req.getServerPath();

	if (loc->at("session") != "off")
		return handleSessionRequest(req, res);
	if (loc->at("cookie") != "off")
		return handleCookieRequest(req, res);
	memset(&st, 0, sizeof(struct stat));
	if (stat(serverPath.c_str(), &st))
		throw(ClientErrorException("file not found: " + serverPath, 404));
	if ((st.st_mode & S_IFMT) == S_IFDIR)
	{
		if (*serverPath.rbegin() != '/')
			res.makeRedirection(req.getUriPath() + '/', 301);
		else if (!loc->at("index").empty())
			res.setPathCode(serverPath + loc->at("index"), 200);
		else if (loc->at("listing") != "off")
			res.makeListing(serverPath, req.getUriPath());
		else
			res.setPathCode(serverPath + "index.html", 200);
	}
	else
		res.setPathCode(serverPath, 200);
}

void Server::put(Request &req, Response &res)
{
	int code = 201;

	std::string file = req.getServerPath();
	if (*file.rbegin() == '/')
		throw(ClientErrorException("cannot put a folder", 409));
	if (access(file.c_str(), W_OK) == 0)
		code = 204;
	std::ofstream outFile(file.c_str(), std::ios::trunc);
	if (!outFile.is_open())
	{
		if (errno == EACCES)
			throw(ClientErrorException("Authorizations: " + file, 403));
		throw(ClientErrorException("could not open for write file: " + file, 500));
	}
	outFile.write(req.getBody().c_str(), req.getBody().size());
	outFile.close();
	res["Content-Location"] = req.getUriPath();
	res.setPathCode(std::string(), code);
}

void Server::del(Request &req, Response &res)
{
	std::string file = req.getServerPath();
	if (*file.rbegin() == '/')
		throw(ClientErrorException("cannot delete a folder:" + file, 409));
	if (remove(file.c_str()) != 0)
	{
		if (errno == 2)
			throw(ClientErrorException("file not found: " + file, 404));
		if (errno == EACCES)
			throw(ClientErrorException("Authorizations: " + file, 403));
		throw(ClientErrorException("could not delete file: " + file, 500));
	}
	res.setPathCode(std::string(), 204);
}

void printValue(std::ostream &os, std::vector<strmap>::iterator &it, const std::string &key)
{
	if (it->find(key) != it->end() && !it->at(key).empty())
		os << std::setw(15) << key << std::setw(15) << it->at(key) << std::endl;
}

std::ostream &operator<<(std::ostream &os, Server &server)
{
	std::ios_base::fmtflags f(std::cout.flags());
	os << std::endl << color::white;
	os << "http://" << server.config["host"] << ":" << server.config["port"] << " | ";
	os << "http://" << server.config["server_name"] << ":" << server.config["port"];
	os << color::reset;
	return os; // comment this to print locations
	if (!server.locations.empty())
	{
		std::cout << std::endl;
		for (std::vector<strmap>::iterator it = server.locations.begin(); it != server.locations.end(); it++)
		{
			os << "────────────────────────────────────────────────────────";
			os << std::endl << std::left;
			printValue(os, it, "name");
			printValue(os, it, "root");
			printValue(os, it, "index");
			printValue(os, it, "redirection");
			printValue(os, it, "listing");
			printValue(os, it, "types");
			printValue(os, it, "cgi");
			printValue(os, it, "cookie");
			printValue(os, it, "session");
		}
	}
	else
		os << "no locations configured" << std::endl;
	std::cout.flags(f);
	return os;
}

const std::string &Server::getConfig(const std::string &key)
{
	return config[key];
}

std::map<short, std::string> *Server::getErrorPages()
{
	return &errorPages;
}

std::string Server::getErrorPage(short code) const
{
	if (errorPages.find(code) != errorPages.end())
		return errorPages.at(code);
	return std::string();
}

char **Server::getEnvp() const
{
	return envp;
}

const std::string &Server::getConfig(const std::string &key) const
{
	return config.at(key);
}

const sockaddr_in &Server::getSockaddrs() const
{
	return sockaddrs;
}
