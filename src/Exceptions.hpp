/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expections.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-tell <ale-tell@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:01:03 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/18 14:01:03 by ale-tell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "webserv.hpp"

class WebservExpection : public std::exception
{
  public:
	WebservExpection(const std::string &msg, short code);
	virtual char const *what() const throw();
	short				getCode() const;

  private:
	char  what_msg[MAX_MSG_LEN];
	short code;
};

class ClientErrorException : public WebservExpection
{
  public:
	ClientErrorException(const std::string &msg, short code) : WebservExpection(msg, code) {};
};

class ServerErrorException : public WebservExpection
{
  public:
	ServerErrorException(const std::string &msg, short code) : WebservExpection(msg, code) {};
};
