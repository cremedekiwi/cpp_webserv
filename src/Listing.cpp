/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-tell <ale-tell@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 09:32:40 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/14 09:32:40 by ale-tell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listing.hpp"
#include "Exceptions.hpp"

#include "webserv.hpp"
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <dirent.h>
#include <set>
#include <sys/stat.h>

std::vector<std::string> Listing::makeDirList(const std::string &serverPath, const std::string &uriPath)
{
	struct stat		 st;
	std::set<std::string> directories;
	std::set<std::string> files;
	std::string		 date;
	std::string		 bu;
	std::string		 name;
	struct dirent	*entry;

	DIR *dir = opendir(serverPath.c_str());
	if (!dir)
	{
		if (errno == 2)
			throw(ClientErrorException("folder not found: " + serverPath, 404));
		if (errno == EACCES)
			throw(ClientErrorException("Authorizations folder: " + serverPath, 403));
		throw(ClientErrorException("opendir failed (listing): " + serverPath, 500));
	}
	while ((entry = readdir(dir)) != NULL)
	{
		std::memset(&st, 0, sizeof(struct stat));
		if (stat(std::string(serverPath + entry->d_name).c_str(), &st))
			throw(ServerErrorException("stat failed", 500));
		if (entry->d_name == std::string("."))
			continue;
		date = ctime(&st.st_mtime);
		name = std::string(entry->d_name) + (entry->d_type == DT_DIR ? "/" : "");
		*date.rbegin() = ' ';
		bu = "<a href=\"" + uriPath + name + "\">" + name + "</a>";
		size_t i;
		for (i = 0; i < 40 - name.size(); i++)
			bu += ' ';
		bu += date;
		for (size_t j = 0; j < (100 - (i + (name + date).size())); j++)
			bu += ' ';
		bu += (entry->d_type == DT_DIR ? "-" : ft_to_string(st.st_size));
		(entry->d_type == DT_DIR ? directories : files).insert(bu);
	}
	closedir(dir);
	std::vector<std::string> ret(files.begin(), files.end());
	ret.insert(ret.begin(), directories.begin(), directories.end());
	return ret;
}

std::string Listing::makeListing(const std::string &serverPath, const std::string &uriPath)
{
	std::string html_listing;
	html_listing = "<!DOCTYPE html>\n";
	html_listing += "<html>\n";
	html_listing += "<head><title>Index of " + serverPath + "</title></head>\n";
	html_listing += "<body>\n";
	html_listing += "<h1>Index of " + serverPath + "</h1><hr><pre>\n";

	std::vector<std::string> dirList = makeDirList(serverPath, uriPath);
	for (std::vector<std::string>::iterator it = dirList.begin(); it != dirList.end(); it++)
		html_listing += *it + "\n";

	html_listing += "</pre><hr></body>\n";
	html_listing += "</html>";

	return html_listing;
}
