/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_codes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-tell <ale-tell@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:02:23 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/10 21:02:23 by ale-tell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#include <map>
#include <string>

std::map<int, std::string> httpCodes;

std::string getHttpIdentifier(const int code)
{
    if (httpCodes.find(code) == httpCodes.end())
        return "";
    return httpCodes[code];
}

void initHTTPIdentifiers()
{
    httpCodes[100] = "Continue";
    httpCodes[101] = "Switching Protocols";
    httpCodes[102] = "Processing";
    httpCodes[200] = "OK";
    httpCodes[201] = "Created";
    httpCodes[202] = "Accepted";
    httpCodes[203] = "Non-Authoritative Information";
    httpCodes[204] = "No Content";
    httpCodes[205] = "Reset Content";
    httpCodes[206] = "Partial Content";
    httpCodes[207] = "Multi-Status";
    httpCodes[208] = "Already Reported";
    httpCodes[226] = "IM Used";
    httpCodes[300] = "Multiple Choices";
    httpCodes[301] = "Moved Permanently";
    httpCodes[302] = "Found";
    httpCodes[303] = "See Other";
    httpCodes[304] = "Not Modified";
    httpCodes[305] = "Use Proxy";
    httpCodes[306] = "Reserved";
    httpCodes[307] = "Temporary Redirect";
    httpCodes[308] = "Permanent Redirect";
    httpCodes[400] = "Bad Request";
    httpCodes[401] = "Unauthorized";
    httpCodes[402] = "Payment Required";
    httpCodes[403] = "Forbidden";
    httpCodes[404] = "Not Found";
    httpCodes[405] = "Method Not Allowed";
    httpCodes[406] = "Not Acceptable";
    httpCodes[407] = "Proxy Authentication Required";
    httpCodes[408] = "Request Timeout";
    httpCodes[409] = "Conflict";
    httpCodes[410] = "Gone";
    httpCodes[411] = "Length Required";
    httpCodes[412] = "Precondition Failed";
    httpCodes[413] = "Request Entity Too Large";
    httpCodes[414] = "Request-URI Too Long";
    httpCodes[415] = "Unsupported Media Type";
    httpCodes[416] = "Requested Range Not Satisfiable";
    httpCodes[417] = "Expectation Failed";
    httpCodes[422] = "Unprocessable Entity";
    httpCodes[423] = "Locked";
    httpCodes[424] = "Failed Dependency";
    httpCodes[425] = "Unassigned";
    httpCodes[426] = "Upgrade Required";
    httpCodes[427] = "Unassigned";
    httpCodes[428] = "Precondition Required";
    httpCodes[429] = "Too Many Requests";
    httpCodes[430] = "Unassigned";
    httpCodes[431] = "Request Header Fields Too Large";
    httpCodes[500] = "Internal Server Error";
    httpCodes[501] = "Not Implemented";
    httpCodes[502] = "Bad Gateway";
    httpCodes[503] = "Service Unavailable";
    httpCodes[504] = "Gateway Timeout";
    httpCodes[505] = "HTTP Version Not Supported";
    httpCodes[506] = "Variant Also Negotiates (Experimental)";
    httpCodes[507] = "Insufficient Storage";
    httpCodes[508] = "Loop Detected";
    httpCodes[509] = "Unassigned";
    httpCodes[510] = "Not Extended";
    httpCodes[511] = "Network Authentication Required";
}
