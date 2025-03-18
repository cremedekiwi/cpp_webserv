/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:03:26 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/25 16:45:46 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Server.hpp"
#include "webserv.hpp"
#include <algorithm>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

// reads a config file and returns a superServerConfig obj
superServerConfig ConfigParser::parse(const char *config_file_name)
{
	superServerConfig ret;
	std::string		  line;
	std::ifstream	  file(config_file_name);

	if (!file.is_open())
		throw std::runtime_error("could not open config file");
	while (std::getline(file, line))
		if (!skipLine(line))
			ret.push_back(parseServer(file));
	return ret;
}

// if line is empty, contains whitespace and start with #, it return true (skip the line)
bool ConfigParser::skipLine(const std::string &line)
{
	if (line.empty())
		return true;
	if (line.find_first_not_of(" \t\n") == line.npos)
		return true;
	if (line[0] == '#')
		return true;
	return false;
}

// skip lines if needed, and extract a pair of key/value
bool ConfigParser::getKeyValue(const std::string &line, std::string &key, std::string &value)
{
	if (skipLine(line))
		return false;
	std::stringstream ss(line);
	ss >> key;
	if (key == "}")
		return false;
	while (whitespace.find(ss.peek()) != line.npos)
		ss.ignore();
	std::getline(ss, value);
	return true;
}

// reads a block of lines from a config file to build a server config (server + location)
// line reading
// key value extraction
// special handling for location
// validation for the keys
// sort location
serverConfig ConfigParser::parseServer(std::ifstream &file)
{
	serverConfig ret;
	std::string	 key(""), value, line;

	while (key != "}" && std::getline(file, line))
	{
		if (!getKeyValue(line, key, value))
			continue;
		else if (key == "location")
			ret.second.push_back(parseLocation(file));
		else if (Server::serverValidKeys.find(key) == Server::serverValidKeys.end())
			throw(std::runtime_error("invalid server's keys in config file"));
		else if (ret.first.find(key) != ret.first.end())
			throw(std::runtime_error("duplicate server's keys in config file"));
		else
			ret.first[key] = value;
	}
	if (!checkConfig(ret.first, Server::serverRequiredKeys))
		throw(std::runtime_error("server missing required key"));
	checkServer(ret);
	std::sort(ret.second.begin(), ret.second.end(), compLocation);
	return ret;
}

locationConfig ConfigParser::parseLocation(std::ifstream &file)
{
	locationConfig ret;
	std::string	   key(""), value, line;

	while (key != "}" && std::getline(file, line))
	{
		if (!getKeyValue(line, key, value))
			continue;
		else if (Server::locationValidKeys.find(key) == Server::locationValidKeys.end())
			throw(std::runtime_error("invalid location's keys in config file"));
		else if (ret.find(key) != ret.end())
			throw(std::runtime_error("duplicate location's keys in config file"));
		else
			ret[key] = value;
	}
	if (!checkConfig(ret, Server::locationRequiredKeys))
		throw(std::runtime_error("location missing required key"));
	checkLocation(ret);
	return ret;
}

bool ConfigParser::checkConfig(const strmap config, std::set<std::string> requiredKeys)
{
	std::set<std::string>::const_iterator it;
	for (it = requiredKeys.begin(); it != requiredKeys.end(); it++)
		if (config.find(*it) == config.end())
			return false;
	return true;
}

void ConfigParser::checkServer(serverConfig &config)
{
	if (ft_stoi<unsigned int>(config.first["port"]) > std::numeric_limits<unsigned short>::max())
		throw std::runtime_error("Invalid port");
	if (ft_stoi<long long int>(config.first["max_body_size"]) <= 0)
		throw std::runtime_error("Invalid max body size");
	if (config.first.find("errror") == config.first.end())
		config.first["root"] = "";
	config.first["host_port"] = config.first["host"] + ":" + config.first["port"];
	if (config.first.find("server_name") != config.first.end())
		config.first["name_port"] = config.first["server_name"] + ":" + config.first["port"];
	else
	{
		config.first["server_name"] = config.first["host"];
		config.first["name_port"] = config.first["host_port"];
	}
}

void ConfigParser::checkLocation(locationConfig &config)
{
	if (*config["name"].begin() != '/')
		throw(std::runtime_error("invalid location's name"));
	if (config.find("root") != config.end() && config.find("redirection") != config.end())
		throw(std::runtime_error("missing root or redirection"));
	if (config.find("root") == config.end())
		config["root"] = "";
	if (config.find("index") == config.end())
		config["index"] = "";
	if (config.find("types") == config.end())
		config["types"] = "GET";
	if (config.find("redirection") == config.end())
		config["redirection"] = "";
	if (config.find("listing") == config.end())
		config["listing"] = "on";
	if (config.find("cgi") == config.end())
		config["cgi"] = "";
	if (config.find("max_body_size") == config.end())
		config["max_body_size"] = "1000000";
	if (config.find("cookie") == config.end())
		config["cookie"] = "off";
	if (config.find("session") == config.end())
		config["session"] = "off";
}

bool ConfigParser::compLocation(const locationConfig &l1, const locationConfig &l2)
{
	if (l1.at("name").size() > l2.at("name").size())
		return true;
	return false;
}
