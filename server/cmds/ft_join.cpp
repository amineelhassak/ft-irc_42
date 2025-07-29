#include "../../headers/server.hpp"


void Server::ft_join(std::vector<std::string> cmds, Server *server, Client &c)
{
	if (cmds.size() < 2) {
		 send_msg(c, ERR_NEEDMOREPARAMS("JOIN"));
		return;
	}

	std::vector<std::string> channel_names = splitByComma(cmds[1]);
	std::vector<std::string> channel_keys;
	if (cmds.size() > 2) {
		channel_keys = splitByComma(cmds[2]);
	}

	for (size_t i = 0; i < channel_names.size(); ++i)
	{
		std::string channel_name = channel_names[i];
		std::string provided_key;
		if (i < channel_keys.size())
			provided_key = channel_keys[i];
		else
			provided_key = "";

        if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&')) {
            send_msg(c, ERR_BADCHANMASK(channel_name));
            continue;
        }
        Channel* cl = NULL;
        for (size_t j = 0; j < allChannels.size(); ++j)
		{
			if (allChannels[j].getName() == channel_name)
			{
                cl = &allChannels[j];
                break;
            }
        }
        // if (!cl)
        // {
        //     send_msg(c,ERR_NOSUCHCHANNEL(channel_name));
        //     return;
        // }
        bool flag = false;
        if (cl) {
            if (cl->hasClient(&c)) {
                send_msg(c, ERR_USERONCHANNEL(c.get_nick(), channel_name));
                continue;
            }
            if (cl->isInviteOnly() && !cl->isInvited(&c)) {
                send_msg(c, ERR_INVITEONLYCHAN(c.get_nick(), channel_name));
                continue;
            }
            if (!cl->getKey().empty() && cl->getKey() != provided_key) {
                send_msg(c, ERR_BADCHANNELKEY(c.get_nick(), channel_name));
                continue;
            }
            if (cl->getUserLimit() > 0 && (int)cl->userCount() >= cl->getUserLimit()) {
                send_msg(c, ERR_CHANNELISFULL(c.get_nick(), channel_name));
                continue;
            }
            cl->addToChannel(&c);
            flag = 1;
        } else {
            allChannels.push_back(Channel(channel_name));
            cl = &allChannels.back();
            cl->addToAdmin(&c);
            cl->addToChannel(&c);
        }
        std::string names_list;
        const std::vector<Client*>& users = cl->getUsers();
        const std::vector<Client*>& admins = cl->getAdmins();
        for (size_t j = 0; j < users.size(); ++j) {
            bool isOp = false;
            for (size_t k = 0; k < admins.size(); ++k) {
                if (admins[k] == users[j]) { isOp = true; break; }
            }
            if (isOp)
                names_list += "@";
            names_list += users[j]->get_nick() + " ";
        }
        std::cout << "User joined" << std::endl;
        for (int i = 0; i < users.size(); i++)
        {
            send_msg(*users[i], ":" + c.get_nick() + "!" + c.get_user() + "@host JOIN :" + channel_name + "\r\n");
        }
        if (flag)
        {
            if (!cl->getTopic().empty())
                send_msg(c, RPL_TOPIC(c.get_nick(), channel_name, cl->getTopic()));
            else
                send_msg(c, RPL_NOTOPIC(c.get_nick(), channel_name));
        }
        send_msg(c, RPL_NAMREPLY(c.get_nick(), channel_name, names_list));
        send_msg(c, RPL_ENDOFNAMES(c.get_nick(), channel_name));
    }
}
