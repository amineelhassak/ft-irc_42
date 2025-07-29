#include "../headers/server.hpp"
#include "../headers/client.hpp"
const char* RESET   = "\033[0m";
const char* BOLD    = "\033[1m";
const char* CYAN    = "\033[36m";
const char* YELLOW  = "\033[33m";
const char* GREEN   = "\033[32m";

std::string get_current_time()
{
	char buff[100];
	std::time_t now = std::time(NULL);
	std::tm* time_info = std::localtime(&now);
	std::strftime(buff, sizeof(buff), "%a %b %d %Y %H:%M:%S", time_info);
	return (std::string(buff));
}

void Server::send_msg(Client& c, std::string msg)
{
	send(c.get_fd(), msg.c_str(), msg.length(), 0);
}





void Server::welcome_msg(Client& c)
{
	std::string nick = c.get_nick();
	std::string user = c.get_user();
	std::string name = this->name;
	std::string version = "1.0";
	std::string date = get_current_time();
	std::string user_modes = "i";
	std::string chan_modes = "it";
	send_msg(c,  std::string (":irc.com ") + "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + name + "\r\n");
	send_msg(c,  std::string (":irc.com ") + "002 " + nick + " :Your host is " + name + ", running version " + version + "\r\n");
	send_msg(c, std::string (":irc.com ") +  "003 " + nick + " :This server was created " + date + "\r\n");
	send_msg(c, std::string (":irc.com ") +  "004 " + nick + " " + name + " " + version + " " + user_modes + " " + chan_modes + "\r\n");
}

std::vector<std::string> split(const std::string& input) {
	std::istringstream iss(input);
	std::vector<std::string> result;
	std::string word;
	while (iss >> word)
		result.push_back(word);
	return result;
}

std::vector<std::string> splitByComma(const std::string &input) {
	std::vector<std::string> tokens;
	std::stringstream ss(input);
	std::string token;
	while (std::getline(ss, token, ',')) {
		tokens.push_back(token);
	}
	return tokens;
}

int Server::existChannel(std::string name)
{
	return 0;
}

void Server::handle_buff_line(Client& c, const std::string& buff)
{
	c.buffer += buff;
	size_t pos;
	if ((pos = c.buffer.find("\n")) == std::string::npos)
	{
		return;
	}
	while ((pos = c.buffer.find("\n")) != std::string::npos)
	{
		std::string line = c.buffer.substr(0, pos);
		c.buffer.erase(0, pos + 1);
		if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
		std::vector<std::string> cmd = split(line);
		execute_cmd(c, cmd);
	}
}

void displayHelps()
{
    std::cout <<
        BOLD << CYAN << "[============= IRC MANUAL =============]" << RESET << "\n"
        << BOLD << "Available Commands:" << RESET << "\n\n"

        << YELLOW << "PASS <password>" << RESET << "\n"
        << "  - Authenticate yourself with the server using a password.\n"
        << "  - Must be sent before NICK/USER if required by the server.\n\n"

        << YELLOW << "USER <username> <unused> <unused> <realname>" << RESET << "\n"
        << "  - Register a new user with the server.\n"
        << "  - Example: USER john localhost server :John Doe\n\n"

        << YELLOW << "NICK <nickname>" << RESET << "\n"
        << "  - Set or change your nickname.\n"
        << "  - Example: NICK coolguy123\n\n"

        << YELLOW << "JOIN <#channel>" << RESET << "\n"
        << "  - Join a channel or create one if it doesn't exist.\n"
        << "  - Example: JOIN #general\n\n"

        << YELLOW << "INVITE <nickname> <#channel>" << RESET << "\n"
        << "  - Invite a user to a channel you are in (must be an operator).\n"
        << "  - Example: INVITE alice #general\n\n"

        << YELLOW << "KICK <#channel> <user> [<reason>]" << RESET << "\n"
        << "  - Remove a user from a channel (must be an operator).\n"
        << "  - Example: KICK #general bob :spamming\n\n"

		<< YELLOW << "MODE <channel> <flags> [<params>]" << RESET << "\n"
		<< "  - Change or view the mode of a channel.\n"
		<< "  - Modes control channel settings (e.g., +i, +t, +o).\n"
		<< "  - Example: MODE #general +o alice  (makes alice an operator)\n"
		<< "  - Example: MODE #general           (shows current modes)\n\n"

		<< YELLOW << "TOPIC <channel> [<topic>]" << RESET << "\n"
		<< "  - View or set the topic of a channel.\n"
		<< "  - Without <topic>: shows the current topic.\n"
		<< "  - With <topic>: sets the new topic (if allowed).\n"
		<< "  - Example: TOPIC #general :Welcome to our channel!\n\n"

		<< YELLOW << "PRIVMSG <target> :<message>" << RESET << "\n"
		<< "  - Send a private message to a user or channel.\n"
		<< "  - <target> can be a nickname or channel name.\n"
		<< "  - Example: PRIVMSG bob :Hello Bob!\n"
		<< "  - Example: PRIVMSG #general :Hello everyone!\n\n"

        << BOLD << CYAN << "[=======================================]" << RESET << "\n";
}
void    Server::init_socket()
{
	struct  sockaddr_in server;
	this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->socket_fd == -1)
	{
		std::cerr << "socket failed!" << std::endl;
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	int opt = 1;
	if (setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		close(this->socket_fd);
		std::cerr << "setsockopt failed!" << std::endl;
		exit(1);
	}

	if (bind(this->socket_fd, (struct sockaddr*)&server, sizeof(server)) != 0)
	{
		close(this->socket_fd);
		std::cerr << "bind failed!" << std::endl;
		exit(1);
	}
	std::cout << "Welcom To Our Server" << std::endl;
    displayHelps();
	if (listen(this->socket_fd, SOMAXCONN) != 0)
	{
		close(this->socket_fd);
		std::cerr << "listen failed!" << std::endl;
		exit(1);
	}
	struct pollfd listener;
	listener.fd = socket_fd;
	listener.events = POLLIN;
	poll_fds.push_back(listener);
	while (true)
	{
		int res;
		res = poll(poll_fds.data(), poll_fds.size(), -1);
		if (res > 0)
		{
			if (poll_fds[0].revents & POLLIN)
			{
				int fd_client;
				fd_client = accept(socket_fd, NULL, NULL);
				if (fd_client < 0)
				{
					std::cerr << "accept failed!" << std::endl;
					continue;
				}
				else
				{
					clients[fd_client] = Client(fd_client);
					pollfd  new_client;
					new_client.fd = fd_client;
					new_client.events = POLLIN;
					poll_fds.push_back(new_client);
					std::cout << "A new client just connected!" << std::endl;
				}
			}
			for (long unsigned int i = 1; i < poll_fds.size(); ++i)
			{
				if (poll_fds[i].revents & POLLIN)
				{
					char buff[512];
					memset(buff, 0, 512);
					ssize_t read_size;
					read_size = recv(poll_fds[i].fd, buff, sizeof(buff), 0);
					if (read_size <= 0)
					{
						close(poll_fds[i].fd);
						poll_fds.erase(poll_fds.begin() + i);
						--i;
						// remove the client from map 
						// std::map<int, Client>clients;

						// function to remove it form all channels 
						continue;
					}
					handle_buff_line(clients[poll_fds[i].fd], buff);
				}
				// else if (poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
				// {
				// 	if (poll_fds[i].revents & POLLHUP)
				// 		std::cerr << "client disconnected (POLLHUP)!" << std::endl;
				// 	if (poll_fds[i].revents & POLLERR)
				// 		std::cerr << "client socket error (POLLERR)!" << std::endl;
				// 	if (poll_fds[i].revents & POLLNVAL)
				// 		std::cerr << "client invalid fd (POLLNVAL)!" << std::endl;
				// 	close(poll_fds[i].fd);
				// 	poll_fds.erase(poll_fds.begin() + i);
				// 	--i;
				// 	continue;
				// }
			}
		}
	}
}

