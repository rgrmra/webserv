# Webserv - HTTP Server in C++98

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

A lightweight, non-blocking HTTP server written in C++98, compliant with HTTP 1.1 standards. Designed to handle static content, file uploads, CGI scripts, and more. Includes bonus features like session management and multiple CGI support.

---

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Testing](#testing)
- [Bonus Features](#bonus-features)
- [Limitations](#limitations)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## Features
- **HTTP/1.1 Compliance**: Supports GET, POST, DELETE methods, chunked transfers, and error handling.
- **Non-Blocking I/O Multiplexing**: Uses `epoll()` for efficient client-server communication.
- **Static Website Hosting**: Serve static files (HTML, CSS, JS, images) with directory listing options.
- **File Uploads**: Accept client file uploads via POST requests.
- **CGI Support**: Execute scripts (e.g., PHP, Python) with proper environment setup.
- **Configurable**: Inspired by NGINX syntax for defining ports, routes, error pages, and client limits.
- **Resilient**: No crashes allowed, even under memory constraints.
- **Bonus**: Session management, cookies, and multiple CGI handlers.

---

## Installation

### Dependencies
- C++98-compatible compiler (e.g., `g++, clang`)
- `make`
- Testing tools: `curl`, web browser, siege, or custom scripts (Python/Go).

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/webserv.git
   cd webserv
   ```
2. Compile using the provided Makefile:
   ```bash
   make
   ```
   Output: `./webserv`

---

## Usage
Run the server with an optional configuration file:
```bash
./webserv [config_file.conf]
```
If no file is provided, the server uses a default configuration (see [`configs/default.conf`](configs/default.conf)).

---

## Configuration
Configure ports, routes, error pages, and more using an NGINX-style file. Example:
```nginx
server {
    listen 8080;
    server_name localhost;
    client_max_body_size 10M;
    
    location /uploads {
        root /var/www;
        methods POST GET;
        upload_enable on;
    }
    
    location /cgi-bin {
        root /var/cgi;
        cgi .py /usr/bin/python3;
        cgi .php /usr/bin/php-cgi;
    }
}
```
Key options:
- `listen`: Port and host (e.g., `127.0.0.1:8080`).
- `client_max_body_size`: Max upload size.
- `methods`: Allowed HTTP methods per route.
- `cgi`: Map file extensions to executables.

---

## Testing
1. **Basic Checks**:
   ```bash
   curl http://localhost:8080
   ```
2. **Stress Testing**:
   Use tools like Apache Bench:
   ```bash
   ab -n 1000 -c 50 http://localhost:8080/
   ```
3. **Browser Testing**: Open `http://localhost:8080` in a browser.

Sample test scripts are provided in [`tests/`](tests/).

---

## Bonus Features
- **Cookies & Sessions**: Track user sessions using cookies. Example config:
  ```nginx
  location /login {
      cookie_session on;
      session_timeout 3600; # 1 hour
  }
  ```
- **Multiple CGI Handlers**: Run Python, PHP, and other scripts simultaneously.

---

## Limitations
- No regex support in route paths.
- Limited to HTTP/1.1 features outlined in the RFC (no WebSocket or HTTP/2).

---

## Contributing
Pull requests are welcome! Ensure code adheres to C++98 standards and passes all tests.

---

## License
Distributed under the MIT License. See [`LICENSE`](LICENSE) for details.

---

## Acknowledgments
- Inspired by NGINX configuration syntax.
- RFC 7230/7231 for HTTP standards.
- 42 School for the project guidelines.
