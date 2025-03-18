/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:39:39 by jarumuga          #+#    #+#             */
/*   Updated: 2025/02/25 17:13:42 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Cookie
{
  public:
	Cookie(const std::string &name, const std::string &value);
	~Cookie();

	std::string getName() const;
	std::string getValue() const;
	std::string getPath() const;
	std::string getDomain() const;
	std::string getExpires() const;
	std::string getSameSite() const;
	bool		isSecure() const;
	bool		isHttpOnly() const;

	void setValue(const std::string &value);
	void setPath(const std::string &path);
	void setDomain(const std::string &domain);
	void setExpires(const std::string &expires);
	void setSameSite(const std::string &sameSite);
	void setSecure(bool secure);
	void setHttpOnly(bool httpOnly);

	std::string toString() const;

  private:
	std::string name_;
	std::string value_;
	std::string path_;
	std::string domain_;
	std::string expires_;
	std::string sameSite_;
	bool		secure_;
	bool		httpOnly_;
};
