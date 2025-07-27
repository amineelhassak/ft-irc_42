#include "../headers/server.hpp"

bool nick_exist (std::map<int, Client> clients, std::string nick)
{
	for (int i =0; i < clients.size(); i++)
	{
		if (clients[i].get_nick() == nick)
			return true;
	}
	return false;
}

bool	check_realname(std::string realname)
{
	if (realname.length() < 2)
		return (false);
	if (!std::isalnum(realname[1]))
		return (false);
	return true;
}

void Server::auth(Client &c, std::vector<std::string> cmd)
{
	static int pass_entered;
	if (cmd[0] == "PASS")
	{
		if (c.get_has_pass())
		{
			send_msg(c, "462 * PASS :You may not reregister\r\n");
			return;
		}
		if (cmd.size() != 2)
		{
			send_msg(c, "461 * PASS :Syntax error\r\n");
			return;
		}
		std::string	pass;
		pass = cmd[1];
		if (pass.empty())
		{
			send_msg(c, "461 * PASS :Not enough parameters\r\n");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * PASS :You may not reregister\r\n");
			return;
		}
		if (password != pass)
		{
			send_msg(c, "464 * PASS :Password incorrect\r\n");
			return;
		}
		c.set_pass(pass);
		c.set_has_pass(true);
	}
	else if (cmd[0] == "NICK")
	{
		if (cmd.size() != 2)
		{
			std::cout << "tanya" << std::endl;
			send_msg(c, "461 * USER :Syntax error\r\n");
			return;
		}
		if (cmd[1][0] == '#')
		{
			send_msg(c, "forbidden chaaracter used\r\n");
			return ;
		}
		std::string	nick;
		nick = cmd[1];
		if (nick_exist(clients, nick))
		{
			send_msg(c, nick + " :Nickname is already in use\r\n");
			return;
		}
		if (nick.empty())
		{
			send_msg(c, "461 * NICK :Not enough parameters\r\n");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * :You may not reregister\r\n");
			return;
		}
		c.set_nick(nick);
		c.set_has_nick(true);
	}
	else if (cmd[0] == "USER")
	{
		if (cmd.size() < 5 || cmd[4].empty() || cmd[4][0] != ':' || !check_realname(cmd[4]))
		{
			send_msg(c, "461 * USER :Syntax error\r\n");
			return;
		}
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
			send_msg(c, "461 * :need more params\r\n");
			return;
		}
		realname = realname.substr(2);
		if (user.empty())
		{
			send_msg(c, "461 * USER :Not enough parameters\r\n");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * :You may not reregister\r\n");
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


std::string up(std::string in)
{
    std::string out(in.size(), ' ');
    for (int i = 0; i < in.size(); i++)
    {
        out[i] = toupper(in[i]);
    }
    return out;
}

void Server::execute_cmd(Client &c, std::vector<std::string> cmdList)
{
	if (!cmdList.size())
		return;
	std::string cmd = cmdList[0];
	// std::cout << cmd << std::endl;
	cmd = up(cmd);
	cmdList[0] = up(cmdList[0]);
	// std::cout << cmd << std::endl;
		
	if (cmd == "PASS" || cmd == "USER" || cmd == "NICK")
		auth(c, cmdList);
	else if (!c.is_registered())
	{
		send_msg(c, "ur not autntified");
		return ;
	}
	else if (cmd == "JOIN")
	{
		ft_join(cmdList, this, c);
	}
	else if (cmd == "MODE")
	{
		ft_mode(cmdList, this, c);
	}
	else if (cmd == "INVITE")
	{
		ft_invite(cmdList, this, c);
	}
	else if (cmd == "KICK")
	{
		ft_kick(cmdList, this, c);
	}
	else if (cmd == "PRIVMSG")
	{
		ft_privmsg(cmdList, this, c);
	}
	else if (cmd == "TOPIC")
	{
		ft_topic(cmdList, this, c);
	}
	else if (cmd == "HELP")
	{
		help(&c);
	}
	else if (cmd == "BOT")
	{
		bot(&c, cmdList);
	}
	else
	{
		send_msg(c, std::string("COMMAND NOT FOUND\r\n"));
	}
}
