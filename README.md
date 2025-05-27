# Webserv 🚀

A custom HTTP/1.1 server implementation written in C++98, designed to handle web requests with modern features while maintaining compatibility with older standards.

![Webserv Banner](https://img.shields.io/badge/HTTP-1.1-blue) ![C++98](https://img.shields.io/badge/C++-98-green)

## ✨ Features

### Core HTTP Server
- **HTTP/1.1 Protocol**: Full implementation with proper request/response handling
- **Multiple HTTP Methods**: GET, POST, PUT, DELETE, HEAD support
- **Virtual Hosts**: Multiple server configurations on different ports/hosts
- **Non-blocking I/O**: Efficient connection handling using select()
- **Chunked Transfer Encoding**: Support for streaming data transfer

### Advanced Functionality
- **CGI Support**: Execute Python (.py) and Shell (.sh) scripts
- **File Upload/Download**: Handle file operations with proper MIME type detection
- **Directory Listing**: Automatic directory browsing with customizable templates
- **Cookie Management**: Set and retrieve HTTP cookies
- **Session Management**: Server-side session handling with unique session IDs
- **URL Encoding/Decoding**: Proper handling of encoded URLs
- **Custom Error Pages**: Configurable error page templates
- **Request Size Limits**: Configurable maximum body size per location

### Configuration
- **Flexible Config File**: Easy-to-read configuration syntax
- **Location-based Rules**: Different settings per URL path
- **Error Page Customization**: Custom HTML error pages
- **Method Restrictions**: Control allowed HTTP methods per location
- **Redirections**: HTTP redirects with proper status codes

## 🚀 Quick Start

### Prerequisites
- C++ compiler supporting C++98 standard (g++, clang++)
- Make build system
- Unix-like operating system (Linux, macOS)

### Installation

```bash
# Clone the repository
git clone <repository-url>
cd webserv

# Compile the project
make

# Run with default configuration
./webserv webserv.conf
```

### Basic Usage

```bash
# Start the server
./webserv webserv.conf

# Test with curl
curl http://localhost:18000/

# Or visit in browser
open http://localhost:18000/
```

## 🔧 Configuration

The server uses a configuration file (default: `webserv.conf`) with the following structure:

```nginx
server {
    server_name www.example.com
    host 127.0.0.1
    port 8080
    max_body_size 10000000
    error 404 rscs/errors/404.html 403 rscs/errors/403.html

    location {
        name /
        root rscs/
        index index.html
        types GET POST HEAD
    }

    location {
        name /api/
        root rscs/api/
        types GET POST PUT DELETE
        max_body_size 1000000
    }

    location {
        name /cgi-bin/
        root rscs/cgi-bin/
        cgi /usr/bin/python3 .py /usr/bin/bash .sh
        types GET POST
    }
}
```

### Configuration Options

#### Server Block
- `server_name`: Virtual host name
- `host`: Binding IP address
- `port`: Listening port
- `max_body_size`: Maximum request body size in bytes
- `error`: Custom error pages (format: `code path`)

#### Location Block
- `name`: URL path pattern
- `root`: Document root directory
- `index`: Default index file
- `types`: Allowed HTTP methods (space-separated)
- `max_body_size`: Location-specific body size limit
- `listing`: Enable/disable directory listing (`on`/`off`)
- `redirection`: HTTP redirect URL
- `cgi`: CGI interpreter configuration (`interpreter extension`)
- `cookie`: Enable cookie handling (`on`/`off`)
- `session`: Enable session management (`on`/`off`)

## 🌐 HTTP Methods

### GET
- Retrieve files and resources
- Directory listing (when enabled)
- CGI script execution

### POST
- Form data submission
- CGI script execution with body data
- Cookie and session handling

### PUT
- File upload and creation
- Update existing resources

### DELETE
- Remove files from server
- Resource cleanup

### HEAD
- Retrieve headers without body
- Resource metadata checking

## 🔌 CGI Support

The server supports CGI (Common Gateway Interface) scripts:

### Supported Interpreters
- **Python**: `.py` files executed with Python interpreter
- **Shell Scripts**: `.sh` files executed with bash

### CGI Environment Variables
The server provides standard CGI environment variables:
- `CONTENT_LENGTH`: Request body size
- `CONTENT_TYPE`: Request content type
- `REQUEST_METHOD`: HTTP method
- `QUERY_STRING`: URL query parameters
- `SCRIPT_NAME`: CGI script name
- `SERVER_NAME`: Server hostname
- `SERVER_PORT`: Server port
- `REMOTE_ADDR`: Client IP address

### Example CGI Script (Python)

```python
#!/usr/bin/env python3
import os
import sys

print("Content-Type: text/html")
print()
print("<html><body>")
print(f"<h1>Hello from CGI!</h1>")
print(f"<p>Method: {os.environ.get('REQUEST_METHOD')}</p>")
print("</body></html>")
```

## 🍪 Cookies & Sessions

### Cookie Management
```javascript
// Client-side cookie handling
function setCookie(name, value) {
    fetch('/cookie/', {
        method: 'POST',
        body: `name=${value}`
    });
}
```

### Session Management
The server automatically creates and manages sessions:
- Unique session IDs generated
- Configurable session duration
- Automatic cleanup of expired sessions

## 📁 Directory Structure

```
webserv/
├── src/                    # Source code
│   ├── main.cpp           # Main entry point
│   ├── Server.cpp         # HTTP server implementation
│   ├── SuperServer.cpp    # Multi-server manager
│   ├── Request.cpp        # HTTP request parser
│   ├── Response.cpp       # HTTP response builder
│   ├── ConfigParser.cpp   # Configuration parser
│   ├── CGI.cpp           # CGI handler
│   ├── Cookie.cpp        # Cookie management
│   ├── SessionManager.cpp # Session handling
│   └── utils/            # Utility functions
├── include/               # Header files
├── rscs/                 # Web resources
│   ├── index.html        # Default homepage
│   ├── pages/           # Static pages
│   ├── cgi-bin/         # CGI scripts
│   ├── errors/          # Error page templates
│   └── listing/         # Directory listing test files
├── Makefile              # Build configuration
├── webserv.conf          # Server configuration
└── README.md            # This file
```

## 🧪 Testing

### Manual Testing
```bash
# Test different HTTP methods
curl -X GET http://localhost:18000/
curl -X POST http://localhost:18000/cgi-bin/post.py -d "name=test"
curl -X PUT http://localhost:18000/listing/newfile.txt -d "content"
curl -X DELETE http://localhost:18000/listing/file.txt

# Test file upload
curl -X PUT http://localhost:18000/listing/upload.txt --data-binary @localfile.txt

# Test chunked encoding
python3 rscs/cgi-bin/chunked.py
```
