#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <ctime>
#include "client.hpp"
#include "channel.hpp"
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <random>
#include "message.hpp"

class Server
{
	private:
	int socket_fd;
	int port;
	std::string	name;
	std::string password;
	std::vector<Channel> allChannels;
	std::vector<struct pollfd> poll_fds;
	std::map<int, Client>clients;

	public:
	Server(int port, std::string password);
	~Server();
    void	checkAdmin(Channel *c);
	void    init_socket();
	void	auth(Client& c, const std::vector<std::string> cmd);
	void	handle_buff_line(Client& c, const std::string& buff);
	void	welcome_msg(Client& c);
	void	send_msg(Client& c, std::string msg);
	void  	ft_join(std::vector<std::string> cmds, Server *server, Client &c);
	void	ft_mode(std::vector<std::string> cmds, Server *server, Client &c);
	void	ft_kick(std::vector<std::string> cmds, Server *server, Client &c);
	void	ft_invite(std::vector<std::string> cmds, Server *server, Client &c);
	void	ft_topic(std::vector<std::string> cmds, Server *server, Client &c);
	void	ft_privmsg(std::vector<std::string> cmds, Server *server, Client &c);
	void 	execute_cmd(Client &c, std::vector<std::string> cmdList);
	int		existChannel(std::string name);
	void 	help(Client *c);
	void 	bot (Client *c, std::vector <std::string> cmd);
	void private_msg (Client& client , std::string msg_text, std::string target , Client &user);
	size_t size() const ;
    Client* getUser(size_t i);
    void addAdmin(Client* client);
};

std::vector<std::string> splitByComma(const std::string &input);

#endif