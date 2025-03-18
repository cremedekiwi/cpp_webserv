/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mimes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-tell <ale-tell@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:49:15 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/08 19:49:15 by ale-tell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>

strmap mimeTypes;

std::string getMimeType(const std::string &file)
{
	size_t point = file.rfind(".");
	if (point == file.npos)
        return "text/plain";
    const std::string extention = file.substr(point , file.size());
    if (mimeTypes.find(extention) == mimeTypes.end())
        return "text/plain";
    return mimeTypes[extention];
}

void initMimeTypes()
{
    mimeTypes[".aac"] = "audio/aac";
    mimeTypes[".abw"] = "application/x-abiword";
    mimeTypes[".apng"] = "image/apng";
    mimeTypes[".arc"] = "application/x-freearc";
    mimeTypes[".avif"] = "image/avif";
    mimeTypes[".avi"] = "video/x-msvideo";
    mimeTypes[".azw"] = "application/vnd.amazon.ebook";
    mimeTypes[".bin"] = "application/octet-stream";
    mimeTypes[".bmp"] = "image/bmp";
    mimeTypes[".bz"] = "application/x-bzip";
    mimeTypes[".bz2"] = "application/x-bzip2";
    mimeTypes[".cda"] = "application/x-cdf";
    mimeTypes[".csh"] = "application/x-csh";
    mimeTypes[".css"] = "text/css";
    mimeTypes[".csv"] = "text/csv";
    mimeTypes[".doc"] = "application/msword";
    mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mimeTypes[".eot"] = "application/vnd.ms-fontobject";
    mimeTypes[".epub"] = "application/epub+zip";
    mimeTypes[".gif"] = "image/gif";
    mimeTypes[".html"] = "text/html";
    mimeTypes[".htm"] = "text/html";
    mimeTypes[".ico"] = "image/vnd.microsoft.icon";
    mimeTypes[".ics"] = "text/calendar";
    mimeTypes[".jar"] = "application/java-archive";
    mimeTypes[".jpeg"] = "image/jpeg";
    mimeTypes[".jpg"] = "image/jpg";
    mimeTypes[".js"] = "text/javascript";
    mimeTypes[".json"] = "application/json";
    mimeTypes[".jsonld"] = "application/ld+json";
    mimeTypes[".midi"] = "audio/x-midi";
    mimeTypes[".mjs"] = "text/javascript";
    mimeTypes[".mp3"] = "audio/mpeg";
    mimeTypes[".mp4"] = "video/mp4";
    mimeTypes[".mpeg"] = "video/mpeg";
    mimeTypes[".mpkg"] = "application/vnd.apple.installer+xml";
    mimeTypes[".odp"] = "application/vnd.oasis.opendocument.presentation";
    mimeTypes[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    mimeTypes[".odt"] = "application/vnd.oasis.opendocument.text";
    mimeTypes[".oga"] = "audio/ogg";
    mimeTypes[".ogv"] = "video/ogg";
    mimeTypes[".ogx"] = "application/ogg";
    mimeTypes[".opus"] = "audio/ogg";
    mimeTypes[".otf"] = "font/otf";
    mimeTypes[".png"] = "image/png";
    mimeTypes[".pdf"] = "application/pdf";
    mimeTypes[".php"] = "application/x-httpd-php";
    mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
    mimeTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mimeTypes[".rar"] = "application/vnd.rar";
    mimeTypes[".rtf"] = "application/rtf";
    mimeTypes[".sh"] = "application/x-sh";
    mimeTypes[".svg"] = "image/svg+xml";
    mimeTypes[".tar"] = "application/x-tar";
    mimeTypes[".tiff"] = "image/tiff";
    mimeTypes[".ts"] = "video/mp2t";
    mimeTypes[".ttf"] = "font/ttf";
    mimeTypes[".txt"] = "text/plain; charset=utf-8";
    mimeTypes[".vsd"] = "application/vnd.visio";
    mimeTypes[".wav"] = "audio/wav";
    mimeTypes[".weba"] = "audio/webm";
    mimeTypes[".webm"] = "video/webm";
    mimeTypes[".webp"] = "image/webp";
    mimeTypes[".woff"] = "font/woff";
    mimeTypes[".woff2"] = "font/woff2";
    mimeTypes[".xhtml"] = "application/xhtml+xml";
    mimeTypes[".xls"] = "application/vnd.ms-excel";
    mimeTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mimeTypes[".zip"] = "application/zip";
    mimeTypes[".7z"] = "application/x-7z-compressed";
}
