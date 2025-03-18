/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 08:36:15 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/27 12:59:40 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "ConfigParser.hpp"
#include "Exceptions.hpp"
#include "Server.hpp"
#include "color.hpp"
#include "webserv.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <ios>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

Request::Request(const struct sockaddr_in &client, const int server_fd)
	: serverFD(server_fd), parsed_len(0), server(NULL), loc(NULL), client_addr(client), raw(""), cgi_script(""),
	  content_length(0), chunked(false)
{
}

bool Request::appendRaw(std::string line)
{
	// std::cout << line;
	raw += line;
	parsed_len += line.size();
	if (raw.find("\r\n\r\n") != raw.npos && method.empty())
		parseHeader();
	if (method.empty())
		return false;
	if (method != "POST" && method != "PUT")
		return true;
	return parsed_len == content_length;
}

void Request::checkedHeader()
{
	if (Server::httpMethods.find(method) == Server::httpMethods.end())
		throw(ServerErrorException("Unsuported method: " + method, 501));
	if (uri.empty())
		throw(ClientErrorException("empty URI", 400));
	if (http != "HTTP")
		throw(ClientErrorException("bad HTTP: ", 400));
	if (ft_stoi<float>(version) > 1.1 + std::numeric_limits<float>::epsilon())
		throw(ServerErrorException("Unsuported HTTP version (" + version + ")", 505));
	if (header_arguments.find("Host") == header_arguments.end())
		throw(ClientErrorException("request missing host specifier", 400));
	if (method == "POST" || method == "PUT")
		if (header_arguments.find("Content-Length") == header_arguments.end())
			throw(ClientErrorException("request content length missing", 411));
}

bool Request::parseHeader()
{
	std::stringstream ssRaw;
	ssRaw << raw;
	raw.clear();

	parseMethod(ssRaw);
	parseURI(ssRaw);
	parseProtocol(ssRaw);
	parseHeaderAgruments(ssRaw);
	setChunked();
	std::getline(ssRaw, raw, '\0');
	if (header_arguments.find("Content-Length") != header_arguments.end())
		content_length = ft_stoi<size_t>(header_arguments["Content-Length"]);
	parsed_len = raw.size();
	return true;
}

bool Request::parseMethod(std::stringstream &ssRaw)
{
	std::getline(ssRaw, method, ' ');
	if (Server::httpMethods.find(method) == Server::httpMethods.end())
		return false;
	return true;
}

bool Request::parseURI(std::stringstream &ssRaw)
{
	std::getline(ssRaw, uri, ' ');
	const strmap &codes = getURLCodes();
	std::string	  p;

	for (size_t i = 0; i < uri.size(); i++)
	{
		p = uri.substr(i, 3);
		if (uri[i] == '%' && codes.find(p) != codes.end())
			uri.replace(i, 3, codes.at(p));
		else if (uri[i] == '+')
			uri[i] = ' ';
	}
	if (uri.empty())
		return false;
	size_t arg_delim = uri.find("?");
	if (arg_delim != uriPath.npos)
		uriPath = uri.substr(0, arg_delim);
	else
		uriPath = uri;
	queryString = uri.substr(arg_delim + 1);
	return true;
}

bool Request::parseProtocol(std::stringstream &ssRaw)
{
	std::string		  str;
	std::stringstream ss;

	std::getline(ssRaw, protocol, '\r');
	ss << protocol;
	std::getline(ss, http, '/');
	if (http != "HTTP")
		return false;
	std::getline(ss, version, '/');
	if (ft_stoi<float>(version) > 1.1 + std::numeric_limits<float>::epsilon())
		return false;
	return true;
}

bool Request::parseHeaderAgruments(std::stringstream &ssRaw)
{
	std::string key;
	std::string str;

	while (std::getline(ssRaw, str) && (str != "\r"))
	{
		std::stringstream line(str);
		std::getline(line, key, ':');
		if (key.empty())
			continue;
		std::getline(line, header_arguments[key], '\n');
		size_t f;
		while ((f = header_arguments[key].find('\r')) != header_arguments[key].npos)
			header_arguments[key].erase(f, 1);
	}
	return true;
}

size_t Request::parseBody(size_t max_body_size)
{
	std::string		  b;
	std::stringstream ssraw;
	if (method == "GET")
		return 0;
	ssraw << raw;
	raw.clear();
	if (ssraw.str().size() == 0)
		return 0;
	if (chunked)
	{
		std::getline(ssraw, b, '\r');
		content_length = ft_stoi<size_t>(b, std::hex);
		std::getline(ssraw, b, '\0');
		if (!b.empty())
			b = b.substr(1, b.size() - 3);
		body += b + '\n';
	}
	else
	{
		content_length = ft_stoi<size_t>(header_arguments["Content-Length"]);
		std::getline(ssraw, b, '\0');
		body = b;
	}
	if (content_length != b.size())
		throw(ClientErrorException("invalid body size", 413));
	if (content_length > max_body_size)
		throw(ClientErrorException("request too long", 413));

	return b.size();
}

std::map<std::string, std::string> Request::parseCookie()
{
	std::map<std::string, std::string> params;

	std::istringstream stream(body);
	std::string		   pair;

	while (std::getline(stream, pair, '&'))
	{
		size_t pos = pair.find('=');
		if (pos != std::string::npos)
		{
			std::stringstream keyStream(pair.substr(0, pos));
			std::stringstream valueStream(pair.substr(pos + 1));

			std::string key, value;
			parseURI(keyStream);
			parseURI(valueStream);

			key = keyStream.str();
			value = valueStream.str();

			params[key] = value;
		}
	}
	return params;
}

std::map<std::string, std::string> Request::parseSession()
{
	std::map<std::string, std::string> cookies;

	std::string cookieHeader = getHeaderArgument("Cookie");
	if (cookieHeader.empty())
		return cookies;

	std::istringstream stream(cookieHeader);
	std::string		   cookiePair;
	while (std::getline(stream, cookiePair, ';'))
	{
		size_t start = cookiePair.find_first_not_of(" ");
		if (start != std::string::npos)
			cookiePair = cookiePair.substr(start);

		size_t pos = cookiePair.find('=');
		if (pos != std::string::npos)
		{
			std::string key = cookiePair.substr(0, pos);
			std::string value = cookiePair.substr(pos + 1);
			cookies[key] = value;
		}
	}
	return cookies;
}

void Request::makeServerPath(const std::string &name, const std::string &root, const std::string &cgi)
{
	serverPath = uriPath;
	serverPath = serverPath.replace(0, name.size(), root);
	size_t i;
	while ((i = serverPath.find("//")) != serverPath.npos)
		serverPath.replace(i, 2, "/");
	file = serverPath;
	file = file.substr(root.size());
	if (cgi.empty())
		return;
	std::vector<std::string> cgis = ft_split(cgi, ' ');
	if (method != "GET" && method != "POST")
		return;
	for (size_t i = 1; i < cgis.size(); i += 2)
	{
		if (file.find(cgis[i], file.size() - cgis[i].size()) != file.npos)
		{
			cgi_folder = serverPath.substr(0, serverPath.rfind("/"));
			cgi_script = serverPath.substr(cgi_folder.size() + 1, serverPath.npos);
			char buffer[1024];
			cgi_exec = realpath(cgis[i - 1].c_str(), buffer);
		}
	}
}

void Request::setChunked()
{
	chunked = false;
	if (header_arguments.find("Transfer-Encoding") == header_arguments.end())
		return;
	const std::string &encoding = header_arguments.at("Transfer-Encoding");
	if (encoding == "chunked")
		chunked = true;
	else if (encoding.find(", chunked") != encoding.npos)
		chunked = true;
	else if (encoding.find("chunked,") != encoding.npos)
		chunked = true;
	else if (encoding.find(", chunked,") != encoding.npos)
		chunked = true;
	else if (encoding.find("chunked") != encoding.npos)
		chunked = true;
}

std::ostream &operator<<(std::ostream &os, Request &request)
{
	os << color::white;
	displayTimestamp(os);
	os << request.server->getConfig("name_port") << " ";
	os << request.method << " [" << request.uriPath << "]";
	os << color::reset;
	return os;
}

const std::string Request::getCookie(const std::string &name) const
{
	std::string cookieHeader = getHeaderArgument("Cookie");
	if (cookieHeader.empty())
		return "";

	std::istringstream stream(cookieHeader);
	std::string		   cookiePair;
	std::string		   key = name + "=";

	while (std::getline(stream, cookiePair, ';'))
	{
		// trim leading whitespace
		size_t start = cookiePair.find_first_not_of(" \t");
		if (start != std::string::npos)
			cookiePair = cookiePair.substr(start);
		else
			cookiePair = "";

		// check if this pair matches the cookie we're looking for
		if (cookiePair.compare(0, key.size(), key) == 0)
		{
			std::string value = cookiePair.substr(key.size());
			return value;
		}
	}
	return "";
}

const std::string Request::getHeaderArgument(const std::string &key) const
{
	if (header_arguments.find(key) != header_arguments.end())
		return header_arguments.at(key);
	return std::string();
}

const std::string &Request::getQueryString() const
{
	return queryString;
}

struct sockaddr_in Request::getClientAddr() const
{
	return client_addr;
}

const std::string &Request::getCGIScript() const
{
	return cgi_script;
}

bool Request::isChunked() const
{
	return chunked;
}

const std::string &Request::getMethod() const
{
	return method;
}

const std::string &Request::getUriPath() const
{
	return uriPath;
}

const std::string &Request::getServerPath() const
{
	return serverPath;
}

const std::string &Request::getBody() const
{
	return body;
}

void Request::setServerPath(const std::string &f)
{
	file = f;
}

int Request::getServerFD() const
{
	return serverFD;
}

Server *Request::getServer() const
{
	return server;
}

void Request::setServer(Server *s)
{
	server = s;
	res.setErrorPages(s->getErrorPages());
	s->findLocation(*this);
}

locationConfig *Request::getLocation() const
{
	return loc;
}

void Request::setLocation(locationConfig *l)
{
	loc = l;
}

Response &Request::getResponse()
{
	return res;
}

void Request::setResponse(const Response &r)
{
	res = r;
}

const std::string &Request::getCGIExec() const
{
	return cgi_exec;
}

const std::string &Request::getCGIFolder() const
{
	return cgi_folder;
}

size_t Request::getContentLength() const
{
	return content_length;
}

int *Request::getCgiPOut() const
{
	return (int *)cgi_p_out;
}

int *Request::getCgiPIn() const
{
	return (int *)cgi_p_int;
}

void Request::setCgiPOut(const int fd[2])
{

	cgi_p_out[0] = fd[0];
	cgi_p_out[1] = fd[1];
}

void Request::setCgiPIn(const int fd[2])
{

	cgi_p_int[0] = fd[0];
	cgi_p_int[1] = fd[1];
}

void Request::cutbody(const ssize_t size)
{
	body = body.substr(size, body.npos);
}
