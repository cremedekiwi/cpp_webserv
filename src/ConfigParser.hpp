/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:01:03 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/13 15:52:53 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include <fstream>
#include <set>
#include <vector>

typedef strmap											locationConfig;
typedef std::pair<strmap, std::vector<locationConfig> > serverConfig;
typedef std::vector<serverConfig>						superServerConfig;

class ConfigParser
{
  public:
	~ConfigParser() {};
	static superServerConfig parse(const char *config_file_name);

  private:
	ConfigParser() {};
	static bool			  skipLine(const std::string &line);
	static bool			  getKeyValue(const std::string &line, std::string &key, std::string &value);
	static locationConfig parseLocation(std::ifstream &file);
	static serverConfig	  parseServer(std::ifstream &file);
	static bool			  checkConfig(const strmap config, std::set<std::string> requiredKeys);
	static void			  checkLocation(locationConfig &config);
	static void			  checkServer(serverConfig &config);
	static bool			  compLocation(const locationConfig &l1, const locationConfig &l2);
};
