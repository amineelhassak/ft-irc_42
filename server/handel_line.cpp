#include "../headers/server.hpp"

void Server::auth(Client &c, std::vector<std::string> cmd)
{
	if (cmd[0] == "PASS")
	{
		if (cmd.size() != 2)
		{
			send_msg(c, "461 * USER :Syntax error");
			return;
		}
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
	else if (cmd[0] == "NICK")
	{
		if (cmd.size() != 2)
		{
			send_msg(c, "461 * USER :Syntax error");
			return;
		}
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
		if (cmd.size() < 5 || cmd[4].empty() || cmd[4][0] != ':')
		{
			send_msg(c, "461 * USER :Syntax error");
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

void Server::execute_cmd(Client &c, std::vector<std::string> cmdList)
{
	if (!cmdList.size())
		return;
	std::string cmd = cmdList[0];

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
		send_msg(c, std::string("COMMAND NOT FOUND"));
	}
}
