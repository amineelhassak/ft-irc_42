# 🚀 ft_irc - IRC Server Implementation

A robust Internet Relay Chat (IRC) server implementation in C++98, developed as part of the 42 curriculum. This project demonstrates advanced socket programming, multi-client handling, and IRC protocol implementation.

## 📋 Table of Contents

- [Features](#-features)
- [Prerequisites](#-prerequisites)
- [Installation](#-installation)
- [Usage](#-usage)
- [IRC Commands](#-irc-commands)
- [Project Structure](#-project-structure)
- [Technical Details](#-technical-details)
- [Contributing](#-contributing)
- [License](#-license)

## ✨ Features

### Core Functionality
- **Multi-client Support**: Handle multiple clients simultaneously using poll()
- **Channel Management**: Create, join, and manage IRC channels
- **User Authentication**: Secure password-based authentication system
- **Real-time Messaging**: Instant message delivery between users and channels

### IRC Commands Implementation
- `/JOIN` - Join channels
- `/PART` - Leave channels  
- `/PRIVMSG` - Send private messages
- `/NOTICE` - Send notices
- `/MODE` - Channel and user mode management
- `/INVITE` - Invite users to channels
- `/KICK` - Remove users from channels
- `/TOPIC` - Set and view channel topics
- `/BOT` - Interactive bot functionality

### Advanced Features
- **Channel Modes**: Support for various channel modes (invite-only, topic protection, etc.)
- **User Modes**: Individual user privilege management
- **Error Handling**: Comprehensive error handling and user feedback
- **Clean Architecture**: Well-structured, maintainable codebase

## 🔧 Prerequisites

- **C++98 compatible compiler** (g++, clang++)
- **Unix-like operating system** (Linux, macOS)
- **Make** utility
- **Basic understanding of IRC protocol**

## 📦 Installation

1. **Clone the repository**
   ```bash
   git clone <your-repo-url>
   cd ft-irc_42
   ```

2. **Compile the project**
   ```bash
   make
   ```

3. **Verify compilation**
   ```bash
   ./ircserv
   ```

## 🚀 Usage

### Starting the Server

```bash
./ircserv <port> <password>
```

**Parameters:**
- `<port>`: Port number for the server (e.g., 6667)
- `<password>`: Server password for client authentication

**Example:**
```bash
./ircserv 6667 mysecretpassword
```

### Connecting with IRC Clients

You can connect to your server using any IRC client:

```bash
# Using netcat for testing
nc localhost 6667

# Using irssi
irssi -c localhost -p 6667 -w mysecretpassword

# Using hexchat or other GUI clients
# Server: localhost
# Port: 6667
# Password: mysecretpassword
```

## 💬 IRC Commands

### Basic Commands

| Command | Description | Example |
|---------|-------------|---------|
| `JOIN` | Join a channel | `JOIN #general` |
| `PART` | Leave a channel | `PART #general` |
| `PRIVMSG` | Send private message | `PRIVMSG #general :Hello everyone!` |
| `NOTICE` | Send notice | `NOTICE #general :Server notice` |

### Channel Management

| Command | Description | Example |
|---------|-------------|---------|
| `MODE` | Set channel/user modes | `MODE #general +i` |
| `INVITE` | Invite user to channel | `INVITE alice #general` |
| `KICK` | Remove user from channel | `KICK #general bob :No reason` |
| `TOPIC` | Set channel topic | `TOPIC #general :Welcome to our channel!` |

### Special Features

| Command | Description | Example |
|---------|-------------|---------|
| `BOT` | Interact with server bot | `BOT help` |

## 📁 Project Structure

```
ft-irc_42/
├── headers/                 # Header files
│   ├── irc.hpp             # Main IRC definitions
│   ├── server.hpp          # Server class definition
│   ├── client.hpp          # Client class definition
│   ├── channel.hpp         # Channel class definition
│   ├── message.hpp         # Message handling
│   └── cmds.hpp            # Command definitions
├── src/                    # Source files
│   ├── main.cpp            # Main entry point
│   └── parse.cpp           # Input parsing
├── server/                 # Server implementation
│   ├── server.cpp          # Server main logic
│   ├── create.cpp          # Server creation
│   ├── handel_line.cpp     # Line handling
│   └── cmds/               # Command implementations
│       ├── bot.cpp         # Bot functionality
│       ├── ft_invite.cpp   # INVITE command
│       ├── ft_join.cpp     # JOIN command
│       ├── ft_kick.cpp     # KICK command
│       ├── ft_mode.cpp     # MODE command
│       ├── ft_privmsg.cpp  # PRIVMSG command
│       └── ft_topic.cpp    # TOPIC command
├── client/                 # Client implementation
│   ├── client.cpp          # Client logic
│   └── Channel.cpp         # Channel management
├── Makefile                # Build configuration
└── README.md               # This file
```

## 🔧 Technical Details

### Architecture
- **Object-Oriented Design**: Clean separation of concerns with Server, Client, and Channel classes
- **Event-Driven**: Uses poll() for efficient I/O multiplexing
- **Protocol Compliant**: Implements standard IRC protocol features

### Key Components

#### Server Class
- Manages socket connections and client handling
- Implements command routing and execution
- Handles channel and user management

#### Client Class
- Represents individual user connections
- Manages user state and authentication
- Handles message buffering and parsing

#### Channel Class
- Manages channel-specific data and users
- Implements channel modes and permissions
- Handles channel-specific operations

### Build System
- **Makefile**: Automated compilation with dependency tracking
- **C++98 Standard**: Ensures broad compatibility
- **Clean Targets**: `make clean`, `make fclean`, `make re`

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is part of the 42 curriculum and is developed for educational purposes.

## 🙏 Acknowledgments

- **42 School** for providing the project requirements
- **IRC Protocol RFC** for protocol specifications
- **C++ Community** for best practices and examples

---

<div align="center">
  <p>Made with ❤️ for the 42 community</p>
  <p>If you find this project helpful, please give it a ⭐</p>
</div>
