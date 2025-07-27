# 💬 FT_IRC - IRC Server Implementation

[![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Complete-brightgreen.svg)]()
[![Build](https://img.shields.io/badge/Build-Passing-success.svg)]()
[![Version](https://img.shields.io/badge/Version-1.0.0-blue.svg)]()

<div align="center">

# 💬 FT_IRC Server

> A complete IRC (Internet Relay Chat) server implementation in C++ with multi-client support.

**Build your own IRC server with full protocol support and multi-client management!**

[![GitHub stars](https://img.shields.io/github/stars/yourusername/ft-irc_42?style=social)](https://github.com/yourusername/ft-irc_42/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/yourusername/ft-irc_42?style=social)](https://github.com/yourusername/ft-irc_42/network)
[![GitHub issues](https://img.shields.io/github/issues/yourusername/ft-irc_42)](https://github.com/yourusername/ft-irc_42/issues)

</div>

---

## 📚 Table of Contents

- [🎯 Overview](#-overview)
- [✨ Features](#-features)
- [🛠️ Installation](#️-installation)
- [🚀 Usage](#-usage)
- [📡 IRC Protocol](#-irc-protocol)
- [📁 Project Structure](#-project-structure)
- [🛠️ Technologies Used](#️-technologies-used)
- [🧪 Testing](#-testing)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

## 🎯 Overview

FT_IRC is a **complete IRC server implementation** written in C++ that supports the Internet Relay Chat protocol. It provides multi-client support, channel management, user authentication, and message broadcasting capabilities.

### 🎓 What You'll Experience

- **Multi-client support** with concurrent connections
- **Channel management** with join/leave operations
- **User authentication** and registration
- **Message broadcasting** to channels and users
- **IRC protocol compliance** with standard commands
- **Real-time communication** between clients

## ✨ Features

### 🖥️ Server Management
- ✅ **Multi-client support** with socket programming
- ✅ **Concurrent connections** using select() or poll()
- ✅ **Port configuration** with command line arguments
- ✅ **Server startup/shutdown** with graceful handling
- ✅ **Error handling** and logging system

### 👥 User Management
- ✅ **User registration** with nicknames and usernames
- ✅ **User authentication** and validation
- ✅ **User status tracking** (online/offline)
- ✅ **User information** storage and retrieval
- ✅ **User disconnection** handling

### 📢 Channel System
- ✅ **Channel creation** and management
- ✅ **Join/leave channels** with proper notifications
- ✅ **Channel operators** with special privileges
- ✅ **Channel modes** and settings
- ✅ **Channel message broadcasting**

### 💬 Communication
- ✅ **Private messaging** between users
- ✅ **Channel messaging** to all channel members
- ✅ **Server notices** and announcements
- ✅ **Message formatting** and delivery
- ✅ **Broadcast capabilities**

### 🔧 IRC Commands
- ✅ **NICK** - Change nickname
- ✅ **USER** - Set user information
- ✅ **JOIN** - Join a channel
- ✅ **PART** - Leave a channel
- ✅ **PRIVMSG** - Send private message
- ✅ **NOTICE** - Send notice
- ✅ **QUIT** - Disconnect from server
- ✅ **PING/PONG** - Keep-alive mechanism

## 🛠️ Installation

### Prerequisites

- **C++ compiler** (GCC 7.3+ or Clang 6.0+)
- **Make** (GNU Make 4.0+)
- **Linux/macOS** (tested on Ubuntu 20.04 and macOS)

### 🏗️ Building the Project

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/ft-irc_42.git
   cd ft-irc_42
   ```

2. **Build the project**
   ```bash
   make
   ```

3. **Build with debug information**
   ```bash
   make debug
   ```

### 🔧 Available Make Commands

| Command | Description |
|---------|-------------|
| `make` | Builds the main server |
| `make debug` | Builds with debug symbols |
| `make clean` | Removes object files |
| `make fclean` | Removes object files and executables |
| `make re` | Rebuilds the project from scratch |

## 🚀 Usage

### 🖥️ Starting the Server

1. **Basic server startup:**
   ```bash
   ./ircserv <port> <password>
   ```

2. **Example with specific port:**
   ```bash
   ./ircserv 6667 password123
   ```

### 👤 Connecting Clients

1. **Using netcat (for testing):**
   ```bash
   nc localhost 6667
   ```

2. **Using IRC client (recommended):**
   - **HexChat** (Linux/macOS/Windows)
   - **mIRC** (Windows)
   - **LimeChat** (macOS)

### 📡 IRC Client Configuration

When connecting with an IRC client:
- **Server**: localhost (or your server IP)
- **Port**: 6667 (or your configured port)
- **Password**: The password you set when starting the server

## 📡 IRC Protocol

### 🔧 Supported Commands

#### User Registration
```irc
NICK <nickname>
USER <username> <hostname> <servername> <realname>
```

#### Channel Operations
```irc
JOIN <channel>
PART <channel> [<message>]
```

#### Messaging
```irc
PRIVMSG <target> :<message>
NOTICE <target> :<message>
```

#### Server Communication
```irc
PING <server>
PONG <server>
QUIT [<message>]
```

### 📊 Message Format

IRC messages follow the standard format:
```
[:prefix] <command> [parameters] [:trailing]
```

**Example:**
```
:user123!~user@localhost PRIVMSG #general :Hello everyone!
```

## 📁 Project Structure

```
ft-irc_42/
├── 📁 src/
│   ├── 📄 main.cpp              # Main program entry point
│   └── 📄 parse.cpp             # Command parsing utilities
├── 📁 server/
│   ├── 📄 server.cpp            # Main server class
│   ├── 📄 create.cpp            # Server creation and setup
│   └── 📄 handel_line.cpp       # Line handling and processing
├── 📁 client/
│   ├── 📄 client.cpp            # Client management
│   └── 📄 Channel.cpp           # Channel class implementation
├── 📁 headers/
│   ├── 📄 client.hpp            # Client class header
│   ├── 📄 channel.hpp           # Channel class header
│   ├── 📄 cmds.hpp              # Command definitions
│   ├── 📄 server.hpp            # Server class header
│   └── 📄 irc.hpp               # Main IRC header
├── 📁 server/cmds/
│   ├── 📄 nick.cpp              # NICK command implementation
│   ├── 📄 user.cpp              # USER command implementation
│   ├── 📄 join.cpp              # JOIN command implementation
│   ├── 📄 part.cpp              # PART command implementation
│   ├── 📄 privmsg.cpp           # PRIVMSG command implementation
│   ├── 📄 notice.cpp            # NOTICE command implementation
│   └── 📄 quit.cpp              # QUIT command implementation
├── 📄 Makefile                  # Build configuration
└── 📄 README.md                 # This file
```

## 🛠️ Technologies Used

### Programming Languages
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![C++11](https://img.shields.io/badge/C++11-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

### Networking
![Socket Programming](https://img.shields.io/badge/Socket_Programming-000000?style=for-the-badge&logo=network&logoColor=white)
![TCP/IP](https://img.shields.io/badge/TCP/IP-000000?style=for-the-badge&logo=protocol&logoColor=white)
![IRC Protocol](https://img.shields.io/badge/IRC_Protocol-000000?style=for-the-badge&logo=chat&logoColor=white)

### Build Tools
![Make](https://img.shields.io/badge/Make-000000?style=for-the-badge&logo=gnu&logoColor=white)
![GCC](https://img.shields.io/badge/GCC-000000?style=for-the-badge&logo=gnu&logoColor=white)

### Development Tools
![Git](https://img.shields.io/badge/Git-F05032?style=for-the-badge&logo=git&logoColor=white)
![VS Code](https://img.shields.io/badge/VS_Code-007ACC?style=for-the-badge&logo=visual-studio-code&logoColor=white)

### Networking Tools
![Netcat](https://img.shields.io/badge/Netcat-000000?style=for-the-badge&logo=terminal&logoColor=white)
![HexChat](https://img.shields.io/badge/HexChat-000000?style=for-the-badge&logo=chat&logoColor=white)

## 🧪 Testing

### 🧪 Manual Testing

1. **Start the server:**
   ```bash
   ./ircserv 6667 password123
   ```

2. **Connect with netcat:**
   ```bash
   nc localhost 6667
   ```

3. **Register a user:**
   ```irc
   NICK testuser
   USER testuser localhost server :Test User
   ```

4. **Join a channel:**
   ```irc
   JOIN #test
   ```

5. **Send a message:**
   ```irc
   PRIVMSG #test :Hello everyone!
   ```

### 🧪 Automated Testing

Create test scripts to verify functionality:

```bash
#!/bin/bash
# test_irc.sh

echo "Testing IRC server..."

# Start server in background
./ircserv 6667 testpass &
SERVER_PID=$!

# Wait for server to start
sleep 2

# Test connection
echo "NICK testuser" | nc localhost 6667
echo "USER testuser localhost server :Test User" | nc localhost 6667

# Cleanup
kill $SERVER_PID
```

### 🧪 Testing Tools

- **Netcat** - Basic connection testing
- **HexChat** - Full IRC client testing
- **Custom test scripts** - Automated testing
- **Wireshark** - Network packet analysis

## 🤝 Contributing

This is a 42 School project, but if you have suggestions or find bugs, feel free to contribute:

### How to Contribute

1. **Fork the repository**
   ```bash
   git clone https://github.com/yourusername/ft-irc_42.git
   ```

2. **Create a feature branch**
   ```bash
   git checkout -b feature/AmazingFeature
   ```

3. **Make your changes**
   - Follow the existing code style
   - Add comments for complex logic
   - Test thoroughly with multiple clients

4. **Commit your changes**
   ```bash
   git commit -m 'Add some AmazingFeature'
   ```

5. **Push to the branch**
   ```bash
   git push origin feature/AmazingFeature
   ```

6. **Open a Pull Request**

### Contribution Guidelines

- Follow the existing code style and conventions
- Add tests for new features
- Update documentation as needed
- Ensure the server compiles without warnings
- Test with multiple IRC clients

## 📄 License

This project is part of the 42 School curriculum. The code is written for educational purposes.

---

<div align="center">

**Made with ❤️ and lots of ☕**

*Build your own IRC server with full protocol support and multi-client management!*

[![GitHub stars](https://img.shields.io/github/stars/yourusername/ft-irc_42?style=social)](https://github.com/yourusername/ft-irc_42/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/yourusername/ft-irc_42?style=social)](https://github.com/yourusername/ft-irc_42/network)
[![GitHub issues](https://img.shields.io/github/issues/yourusername/ft-irc_42)](https://github.com/yourusername/ft-irc_42/issues)

**Author**: amine el hassak && mohammed el amarty && kaltoum el mounjid
**School**: 42 School  
**Project**: FT_IRC  
**Last updated**: December 2024

</div>
