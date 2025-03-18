/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:54:57 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/26 14:11:25 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Cookie.hpp"
#include "webserv.hpp"
#include <string>
#include <unistd.h>

class Response
{

  public:
	Response() {};
	Response(std::map<short, std::string> *err);
	Response(const Response &ref);
	Response &operator=(const Response &ref);
	~Response() {};
	const std::string		  &getRaw() const;
	void					   makeListing(const std::string &serverPath, const std::string &root);
	void					   makeRedirection(const std::string &path, const int code);
	void					   serializeResponse(bool add_body = true);
	void					   addResponseArgument();
	void					   addBody(const std::string a_file = "");
	void					   addCookie(const Cookie &cookie);
	const std::vector<Cookie> &getCookies() const;
	void					   serializeError(const short errorCode, bool add_body);
	void					   cutRaw(const ssize_t size);
	void					   setResponseArgument(const std::string &key, const std::string &value);
	std::string				  &operator[](const std::string &key);
	friend std::ostream		  &operator<<(std::ostream &os, Response &res);
	void					   setPathCode(const std::string &path, const int code);
	void					   setErrorPages(std::map<short, std::string> *err);
	void					   appendRaw(std::string str);
	void					   setContent(const std::string &content_type, const std::string &content, int status_code);

  private:
	std::string					  raw;
	std::string					  file;
	std::string					  html_listing;
	std::string					  content_;
	short						  code;
	strmap						  arguments;
	bool						  ready;
	std::map<short, std::string> *errorPages;
	std::vector<Cookie>			  cookies_;
};
