/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:43:26 by jarumuga          #+#    #+#             */
/*   Updated: 2025/02/26 14:46:45 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookie.hpp"
#include <sstream>

Cookie::Cookie(const std::string &name, const std::string &value)
	: name_(name), value_(value), path_("/"), domain_(""), expires_(""), secure_(false), httpOnly_(true)
{
}

Cookie::~Cookie() {}

std::string Cookie::getName() const
{
	return name_;
}

std::string Cookie::getValue() const
{
	return value_;
}

std::string Cookie::getPath() const
{
	return path_;
}

std::string Cookie::getDomain() const
{
	return domain_;
}

std::string Cookie::getExpires() const
{
	return expires_;
}

std::string Cookie::getSameSite() const
{
	return sameSite_;
}

bool Cookie::isSecure() const
{
	return secure_;
}

bool Cookie::isHttpOnly() const
{
	return httpOnly_;
}

void Cookie::setValue(const std::string &value)
{
	value_ = value;
}

void Cookie::setPath(const std::string &path)
{
	path_ = path;
}

void Cookie::setDomain(const std::string &domain)
{
	domain_ = domain;
}

void Cookie::setExpires(const std::string &expires)
{
	expires_ = expires;
}

void Cookie::setSameSite(const std::string &sameSite)
{
	sameSite_ = sameSite;
}

void Cookie::setSecure(bool secure)
{
	secure_ = secure;
}

void Cookie::setHttpOnly(bool httpOnly)
{
	httpOnly_ = httpOnly;
}

std::string Cookie::toString() const
{
	std::ostringstream oss;
	oss << name_ << "=" << value_;
	if (!expires_.empty())
		oss << "; Expires=" << expires_;
	if (!path_.empty())
		oss << "; Path=" << path_;
	if (!domain_.empty())
		oss << "; Domain=" << domain_;
	if (secure_)
		oss << "; Secure";
	if (httpOnly_)
		oss << "; HttpOnly";
	return oss.str();
}
