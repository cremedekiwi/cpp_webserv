# webserv

## Overview

Webserv is a custom HTTP server implementation written in C++98. This project aims to recreate the basic functionality of web servers like Nginx, allowing for HTTP request handling, configuration parsing, CGI execution, and more.

## Features

- HTTP/1.1 protocol implementation
- Support for GET, POST, DELETE, and PUT methods
- Custom server configuration file
- Directory listing
- Error page handling
- CGI script execution (.py, .sh)
- File upload handling
- Cookie and session management
- URL encoding/decoding support
- MIME type detection
- Chunked transfer encoding

## Installation

Clone the repository and compile the project:

```bash
git clone <repository-url>
cd webserv
make
```

## Usage

Run the server with a configuration file:

```bash
./webserv [configuration_file]
```

## Configuration

The server configuration is defined in `webserv.conf`. This file allows you to specify:

- Server listen ports
- Server names
- Root directories
- Default error pages
- Client body size limits
- CGI file extensions
- Directory listings
- Allowed HTTP methods per location

Example configuration:

```
server {
    listen 8080;
    server_name localhost;
    root ./rscs;
    error_page 404 ./rscs/errors/404.html;
    error_page 403 ./rscs/errors/403.html;
    client_max_body_size 10M;

    location / {
        index index.html;
        allow_methods GET POST;
    }

    location /upload {
        allow_methods GET POST PUT;
        upload_dir ./rscs/upload;
    }

    location /cgi-bin {
        allow_methods GET POST;
        cgi_pass .py .sh;
    }

    location /listing {
        autoindex on;
        allow_methods GET;
    }
}
```

## HTTP Methods

- **GET**: Retrieve a resource
- **POST**: Submit data to be processed
- **DELETE**: Remove a specified resource
- **PUT**: Upload a resource

## CGI Support

Webserv supports CGI scripts. CGI scripts can be placed in the `rscs/cgi-bin/` directory and accessed via the defined location in the configuration file.

## Error Handling

Custom error pages can be defined in the configuration file. The server comes with default error pages for common HTTP status codes (403, 404).
