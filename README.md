# Webserv - HTTP Server Implementation

A custom HTTP server implementation written in C++98 that mimics nginx functionality with support for multiple virtual hosts, CGI execution, file uploads, and advanced routing.

## Features

### Core HTTP Server Functionality
- **HTTP/1.1 Protocol Support** - Handles GET, POST, and DELETE methods
- **Non-blocking I/O** - Uses epoll for efficient connection management
- **Multiple Virtual Hosts** - Support for multiple servers on different ports
- **Custom Configuration** - nginx-style configuration file parsing
- **Error Handling** - Custom error pages with fallback defaults
- **Static File Serving** - Efficient file serving with proper MIME types

### Advanced Features
- **CGI Support** - Execute Python, PHP, and Shell scripts
- **File Upload/Download** - Multipart form data handling
- **Auto-indexing** - Directory listing with styled HTML
- **Location-based Routing** - URL path-specific configurations
- **Cookie Support** - Session management and authentication
- **Redirections** - HTTP redirects with proper status codes
- **Request Body Size Limits** - Configurable upload size restrictions

### Security & Performance
- **Signal Handling** - Graceful shutdown on SIGINT/SIGQUIT
- **Timeout Management** - CGI timeout protection (5 seconds)
- **Access Control** - Method restrictions per location
- **Resource Management** - Proper cleanup and memory management

## Architecture

### Core Components

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   GlobalData    │────│     Server      │────│     Client      │
│   (epoll mgmt)  │    │  (config + IO)  │    │  (connections)  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │                       │                       │
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│     Parser      │    │    Request      │    │    Response     │
│ (config files)  │    │ (HTTP parsing)  │    │ (HTTP output)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Key Classes
- **GlobalData**: Manages epoll events and server lifecycle
- **Server**: Handles individual server instances and client connections
- **Client**: Represents individual client connections
- **Request**: Parses and manages HTTP requests
- **Parser**: Parses nginx-style configuration files
- **RawBits**: Handles binary data and file uploads

## Configuration

The server uses an nginx-style configuration format:

```nginx
server {
    listen 8080 8081;                    # Multiple ports
    server_names localhost example.com;   # Virtual hosts
    root /var/www/html;                  # Document root
    index index.html index.php;         # Default files
    auto_index on;                       # Directory listing
    allowed_methods GET POST DELETE;     # Allowed HTTP methods
    client_max_body_size 1024;          # Upload limit (bytes)
    upload_folder /uploads;              # Upload directory
    
    # CGI configuration
    cgi .py /usr/bin/python3;
    cgi .php /usr/bin/php;
    cgi .sh /bin/bash;
    
    # Error pages
    error_page 404 /errors/404.html;
    error_page 500 /errors/500.html;
    
    # Location-specific settings
    location /api {
        allowed_methods GET POST;
        cgi .py /usr/bin/python3;
        client_max_body_size 2048;
    }
    
    location /admin {
        internal;                        # Internal access only
        allowed_methods GET;
    }
    
    location /redirect {
        redirect https://example.com;    # HTTP redirect
    }
}
```

## Building and Running

### Prerequisites
- C++98 compatible compiler (g++/clang++)
- Linux system with epoll support
- Make build system

### Build
```bash
# Build the project
make

# Build with Docker
make docker

# Clean build files
make clean
make fclean
```

### Usage
```bash
# Run with default configuration
./webserv

# Run with custom configuration
./webserv custom.conf

# Example with Docker
docker-compose up
```

## Testing

### Built-in Test Pages
- **Main Page**: `/` - Navigation hub
- **File Upload**: `/pages/upload.html` - Upload/delete interface  
- **CGI Scripts**: `/tests/` - Various CGI examples
- **Fractal Demo**: `/pages/fractal.html` - Visual demonstration
- **Code Terminal**: `/pages/code.html` - Interactive programming info

### CGI Scripts
- **Calculator**: `calculate.php` - Basic arithmetic operations
- **Health Analysis**: `analysis.py` - BMI and health calculations  
- **Time Display**: `time.py` - Server time with refresh
- **Shell Script**: `test.sh` - Basic shell CGI example

### Load Testing
```bash
# Python test script for concurrent requests
python3 test.py

# Manual testing
curl -X GET http://localhost:8080/
curl -X POST -F "file=@test.txt" http://localhost:8080/uploads/
curl -X DELETE http://localhost:8080/uploads/test.txt
```

## Project Structure

```
webserv/
├── srcs/                           # Source code
│   ├── main.cpp                   # Entry point
│   ├── class/                     # Class implementations
│   │   ├── GlobalData/           # Event loop management
│   │   ├── Server/               # Server logic & HTTP handling
│   │   ├── Client/               # Client connection management
│   │   ├── Request/              # HTTP request parsing
│   │   ├── Parser/               # Configuration parsing
│   │   └── RawBits/              # Binary data handling
│   └── Utils/                    # Utility functions
├── includes/                     # Header files
├── URIs/                        # Web content & test pages
├── URIs2/                       # Secondary content root
├── serverDefaultPages/          # Default error pages
├── configExample.conf           # Example configuration
└── Makefile                     # Build system
```

## Key Features in Detail

### CGI Implementation
- **Process Management**: Fork/exec with pipe communication
- **Environment Variables**: Proper CGI environment setup
- **Timeout Protection**: 5-second execution limit with SIGALRM
- **Content-Length Injection**: Automatic header completion
- **Multiple Languages**: Python, PHP, Shell script support

### File Upload System
- **Multipart Parsing**: RFC-compliant multipart/form-data handling
- **Boundary Detection**: Automatic boundary extraction
- **Streaming Upload**: Memory-efficient large file handling  
- **Size Limits**: Configurable upload size restrictions
- **File Management**: Upload/download/delete operations

### Authentication System
- **Cookie-based Auth**: Simple session management
- **Protected Routes**: `/auth/secret` requires authentication
- **Login Flow**: `/auth/login` sets authentication cookies

### Auto-indexing
- **Styled Directory Listings**: Custom HTML with CSS styling
- **Navigation**: Clickable file/directory links
- **Responsive Design**: Mobile-friendly interface

## HTTP Compliance

### Supported Methods
- **GET**: Static files, CGI scripts, directory listings
- **POST**: File uploads, form submissions, CGI data
- **DELETE**: File removal operations

### Status Codes
- **2xx Success**: 200 OK, 201 Created, 204 No Content
- **3xx Redirection**: 301/302 redirects, 304 Not Modified  
- **4xx Client Error**: 400 Bad Request, 403 Forbidden, 404 Not Found, 405 Method Not Allowed, 413 Payload Too Large
- **5xx Server Error**: 500 Internal Server Error, 502 Bad Gateway, 504 Gateway Timeout

### Headers
- **Request Processing**: Host, Content-Length, Content-Type, Cookie
- **Response Generation**: Content-Type, Content-Length, Set-Cookie, Location

## Error Handling

### Graceful Degradation
- **Configuration Errors**: Detailed parsing error messages
- **Runtime Errors**: Proper HTTP error responses
- **Resource Limits**: Memory and connection management
- **CGI Failures**: Timeout and execution error handling

### Custom Error Pages
- **Themed Error Pages**: Space-themed 404/403/500 pages
- **Fallback System**: Default pages when custom pages unavailable
- **Location-specific**: Different error pages per location block

## Development Notes

### C++98 Compliance
- **No Modern Features**: STL containers, traditional pointers
- **Manual Memory Management**: Explicit new/delete operations
- **RAII Principles**: Constructor/destructor resource management

### Performance Considerations  
- **Non-blocking I/O**: Epoll-based event handling
- **Efficient Parsing**: Streaming request/response processing
- **Resource Pooling**: Connection reuse and proper cleanup

### Security Features
- **Path Traversal Protection**: `..` sequence detection
- **Method Restrictions**: Per-location HTTP method limits
- **Internal Locations**: Access control for sensitive paths
- **Upload Validation**: File type and size restrictions
