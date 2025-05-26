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
- Testing tools: `curl`, web browser, custom scripts (Python/Go), `siege`.

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/rgrmra/webserv.git
   cd webserv
