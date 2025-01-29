# HTTP Server (C++17)

## Overview
This project is a lightweight HTTP server implemented in C++17. It provides a simple and efficient way for users to handle HTTP requests and responses with support for multi-threading to manage multiple client connections concurrently.

## Features
- Handles basic HTTP requests and responses.
- Supports multi-threaded client connections.
- Uses synchronous resources for efficient request handling.
- Implements custom routing for different endpoints.

## Technologies Used
- C++17
- POSIX Sockets
- Multi-threading (std::thread)
- Synchronization primitives (mutex, condition_variable)

## Installation & Compilation
### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake

### Build Instructions
```sh
git clone https://github.com/MahmoudAbou-Hawis/Http-Server-CPP-17.git
cd Http-Server-CPP-17
mkdir build && cd build
cmake ..
make
```

## Usage
### Running the Server
```sh
./CPP_Server 
```

## Future Improvements
- More advanced request parsing and response formatting

## License
This project is open-source and available under the MIT License.

## Author
[Mahmoud Abou-Hawis](https://github.com/MahmoudAbou-Hawis)

