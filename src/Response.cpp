/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:55:08 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/27 12:59:51 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Exceptions.hpp"
#include "Listing.hpp"
#include "color.hpp"
#include "webserv.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>

Response::Response(std::map<short, std::string> *err) : raw(""), errorPages(err) {}

Response::Response(const Response &ref) : errorPages(ref.errorPages) {}

Response &Response::operator=(const Response &ref)
{
	this->errorPages = ref.errorPages;
	return *this;
}

void Response::makeRedirection(const std::string &path, const int c)
{
	code = c;
	arguments["Location"] = path;
}

void Response::makeListing(const std::string &serverPath, const std::string &root)
{
	html_listing = Listing::makeListing(serverPath, root);
	arguments["Content-Type"] = "text/html";
	code = 200;
}

void Response::serializeResponse(bool add_body)
{
	raw = "HTTP/1.1 " + ft_to_string(code) + " " + getHttpIdentifier(code) + "\r\n";
	if (!file.empty())
		arguments["Content-Type"] = getMimeType(file);
	addResponseArgument();
	raw += "\r\n";
	if (add_body)
		addBody();
}

void Response::serializeError(const short errorCode, bool add_body)
{
	code = errorCode;
	const std::string codestr = ft_to_string(errorCode) + " " + getHttpIdentifier(errorCode);
	raw = "HTTP/1.1 " + codestr + "\r\n";
	arguments["Content-Type"] = "text/html";
	raw += "\r\n";
	if (!add_body)
		return;
	if (errorPages->find(errorCode) != errorPages->end())
		raw += errorPages->at(errorCode);
	else
	{
		raw += "<html><head>";
		raw += "<meta http-equiv=\"content-type\" content=\"text/html; charset=windows-1252\">";
		raw += "<title>";
		raw += codestr;
		raw += "</title></head>";
		raw += "<body>";
		raw += "<center><h1>";
		raw += codestr;
		raw += "</h1></center>";
		raw += "<hr><center>werbserv/1.0</center>";
		raw += "</body></html>";
	}
}

void Response::setContent(const std::string &content_type, const std::string &content, int status_code)
{
	code = status_code;
	arguments["Content-Type"] = content_type;
	arguments["Content-Length"] = ft_to_string(content.length());
	content_ = content;
}

void Response::addBody(const std::string a_file)
{
	if (!a_file.empty())
		file = a_file;
	if (!content_.empty())
		raw += content_;
	if (!file.empty())
	{
		std::ifstream ssResource(file.c_str(), std::ios::binary);
		if (!ssResource.is_open())
		{
			if (errno == 2)
				throw(ClientErrorException("File not found: " + file, 404));
			if (errno == 13)
				throw(ClientErrorException("Permission denied:" + file, 403));
			throw(ClientErrorException("Could not open for write file: " + file, 500));
		}
		raw += std::string((std::istreambuf_iterator<char>(ssResource)), std::istreambuf_iterator<char>());
	}
	else if (!html_listing.empty())
		raw += html_listing;
}

void Response::addResponseArgument()
{
	for (std::map<std::string, std::string>::const_iterator it = arguments.begin(); it != arguments.end(); it++)
		raw += it->first + ": " + it->second + "\r\n";
	for (std::vector<Cookie>::const_iterator it = cookies_.begin(); it != cookies_.end(); ++it)
		raw += "Set-Cookie: " + it->toString() + "\r\n";
}

const std::string &Response::getRaw() const
{
	return raw;
}

void Response::cutRaw(const ssize_t size)
{
	// std::cout << raw.substr(0, size);
	raw = raw.substr(size, raw.npos);
}

void Response::setPathCode(const std::string &path, const int c)
{
	code = c;
	file = path;
}

std::string &Response::operator[](const std::string &key)
{
	if (arguments.find(key) == arguments.end())
		arguments[key] = "";
	return arguments[key];
}

void Response::setErrorPages(std::map<short, std::string> *err)
{
	errorPages = err;
}

std::ostream &operator<<(std::ostream &os, Response &res)
{
	os << color::green;
	displayTimestamp(os);
	os << ft_to_string(res.code);
	if (!res.file.empty())
		os << " " << res.file;
	os << color::reset;
	return os;
}

void Response::addCookie(const Cookie &cookie)
{
	cookies_.push_back(cookie);
}

const std::vector<Cookie> &Response::getCookies() const
{
	return cookies_;
}

void Response::appendRaw(std::string str)
{
	raw += str;
}
