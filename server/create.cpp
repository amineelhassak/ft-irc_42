#include "../headers/server.hpp"
#include "../headers/client.hpp"

std::string	get_current_time()
{
	char	buff[100];
	std::time_t now = std::time(NULL);
	std::tm* time_info = std::localtime(&now);
	std::strftime(buff, sizeof(buff), "%a %b %d %Y %H:%M:%S", time_info);
	return (std::string(buff));
}

void	Server::send_msg(Client& c, std::string msg)
{
	std::string	message;

	message = ":" + this->name + " " + msg + "\r\n";
	send(c.get_fd(), message.c_str(), message.length(), 0);
}

void	Server::welcome_msg(Client& c)
{
	std::string nick = c.get_nick();
	std::string	user = c.get_user();
	std::string name = this->name;
	std::string	version = "1.0";
	std::string date = get_current_time();
	std::string	user_modes = "i";
	std::string	chan_modes = "it";
	send_msg(c, "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + name);
	send_msg(c, "002 " + nick + " :Your host is " + name + ", running version " + version);
	send_msg(c, "003 " + nick + " :This server was created " + date);
	send_msg(c, "004 " + nick + " " + name + " " + version + " " + user_modes + " " + chan_modes);
}



void	Server::handle_line(Client& c, std::vector<std::string> cmd)
{
	// std::stringstream	ss;
	// ss << line;
	// std::string	cmd;
	// ss >> cmd;
	// std::cout << "line : " << line;
	if (cmd.size() == 2 && cmd[0] == "PASS")
	{
	std::cout << "ah ana hna "  << std::endl;
		std::string	pass;
		pass = cmd[1];
		if (pass.empty())
		{
			send_msg(c, "461 * PASS :Not enough parameters");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * :You may not reregister");
			return;
		}
		if (password != pass)
		{
			send_msg(c, "464 * :Password incorrect");
			return;
		}
		c.set_pass(pass);
	}
	else if (cmd.size() == 2 && cmd[0] == "NICK")
	{
		std::string	nick;
		nick = cmd[1];
		if (nick.empty())
		{
			send_msg(c, "461 * NICK :Not enough parameters");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * :You may not reregister");
			return;
		}
		c.set_nick(nick);
		c.set_has_nick(true);
	}
	else if (cmd[0] == "USER")
	{
		std::string user = cmd[1];
		std::string param1 = cmd[2];
		std::string param2 = cmd[3];
		std::string realname;
		for (size_t i = 4; i < cmd.size(); ++i)
		{
			realname += cmd[i];
			if (i != cmd.size() - 1)
				realname += " ";
		}
		if(realname.empty())
		{
			send_msg(c, "461 * :need more params");
			return;
		}
		realname = realname.substr(2);
		if (user.empty())
		{
			send_msg(c, "461 * USER :Not enough parameters");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * :You may not reregister");
			return;
		}
		c.set_user(user);
		c.set_realname(realname);
		c.set_has_user(true);
	}
	if (c.get_has_nick() && c.get_has_user() && !c.is_registered())
	{
		c.set_registered(true);
		welcome_msg(c);
	}
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

int	Server::existChannel(std::string name)
{
	// for(int i = 0;i < allChannels.size();i++)
	// {
	// 	if (allChannels[i].getName() == name)
	// 		return 1;
	// }
	return 0;
}



void Server::ft_join(std::vector<std::string> cmds, Server *server, Client *c)
{
    std::vector<std::string> channels = splitByComma(cmds[1]);
    std::vector<std::string> passwords = splitByComma(cmds[2]);

    for (size_t i = 0; i < channels.size(); ++i)
    {
        const std::string &name = channels[i];

        if (name.empty() || (name[0] != '#' && name[0] != '&')) {
            std::cout << "ERROR 1" << std::endl;
            continue;
        }

        if (existChannel(name)) {
            std::cout << "EXISTE" << std::endl;
        } else {
            Channel cl(name);
            this->allChannels.push_back(cl);
	
			cl.addToAdmin(c);
			cl.addToChannel(c);
        }
    }
}


void Server::cmds(std::vector<std::string> cmds, Server* server, Client* c) {
	std::string keyword = cmds[0];
    // Dispatch vers les commandes IRC principales
    if (keyword == "JOIN") {
        ft_join(cmds, server, c);
    }
    else if (keyword == "MODE") {
        std::cout << "MODE" << std::endl;
    }
    else if (keyword == "INVITE") {
        ft_invite(cmds,server,c);
    }
    else if (keyword == "KICK") {
        std::cout << "KICK" << std::endl;
    }
    else if (keyword == "PRIVMSG") {
        std::cout << "PRIVMSG" << std::endl;
    }
    else if (keyword == "TOPIC") {
        std::cout << "TOPIC" << std::endl;
    }
    else {
        
    }
}




void	Server::handle_buff_line(Client& c, const std::string& buff)
{
	// std::cout << "entred" << buff <<  std::endl;
	c.buffer += buff;
	std::vector<std::string> cmd = split(buff);
	handle_line(c,cmd);
	cmds(cmd, this, &c);
}


void    Server::init_socket()
{
	struct	sockaddr_in server;

	this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->socket_fd == -1)
	{
		std::cerr << "socket failed!" << std::endl;
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->socket_fd, (struct sockaddr*)&server, sizeof(server)) != 0)
	{
		std::cerr << "bind failed!" << std::endl;
		exit(1);
	}
	if (listen(this->socket_fd, SOMAXCONN) != 0)
	{
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
					pollfd	new_client;
					new_client.fd = fd_client;
					new_client.events = POLLIN;
					poll_fds.push_back(new_client); //the new client have just added
					std::cout << "A new client just connected!" << std::endl;
				}
			}
			for (long unsigned int i = 1; i < poll_fds.size(); ++i)
			{
				if (poll_fds[i].revents & POLLIN)
				{
					char buff[512];
					ssize_t read_size;
					read_size = recv(poll_fds[i].fd, buff, sizeof(buff), 0);
					if (read_size <= 0)
					{
						close(poll_fds[i].fd);
						poll_fds.erase(poll_fds.begin() + i);
						--i;
						continue;
					}
					buff[read_size] = '\0';
					
					// std::cout << "buff2 : ";
					// std::cout << "buff : " << buff << std::endl ;
					// std::cout << "buff3 : ";
					// std::cout.flush();
					handle_buff_line(clients[poll_fds[i].fd], buff);
				}
				else if (poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
				{
					std::cerr << "client error!" << std::endl;
					close(poll_fds[i].fd);
					poll_fds.erase(poll_fds.begin() + i);
					--i;
					continue;
				}
			}
		}
	}
}

