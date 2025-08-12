#include "../../headers/server.hpp"

void Channel::removeFromAdmin(Client* client) {
    std::vector<Client*>& admins = this->admins;  // assuming admins is a member variable

    admins.erase(std::remove(admins.begin(), admins.end(), client), admins.end());
}

time_t Channel::getCreationTime() const {
    return creationTime;
}

void Server::ft_mode(std::vector<std::string> cmds, Server* server, Client& c) {
    if (cmds.size() < 2) {
        send_msg(c, ERR_NEEDMOREPARAMS(std::string("MODE")));
        return;
    }

    std::string channelName = cmds[1];
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        send_msg(c, ERR_BADCHANMASK(channelName));
        return;
    }
    Channel* channel = NULL;
    for (size_t i = 0; i < allChannels.size(); ++i) {
        if (allChannels[i].getName() == channelName) {
            channel = &allChannels[i];
            break;
        }
    }
    if (!channel) {
        send_msg(c, ERR_NOSUCHCHANNEL(channelName));
        return;
    }
    if (!channel->hasClient(&c)) {
        send_msg(c, ERR_NOTONCHANNEL(c.get_nick(), channelName));
        return;
    }
    if (cmds.size() < 3) {
        std::ostringstream oss;
        oss << channel->getCreationTime();
        if(!channel->getKey().empty() || channel->getUserLimit() > 0 || channel->isInviteOnly() || channel->isTopicRestricted()) {
        std::cout << "ahah ana kayn" << std::endl;
            std::string modes = "+";
            if (channel->isInviteOnly()) modes += "i";
            if (channel->isTopicRestricted()) modes += "t";
            if (!channel->getKey().empty()){
                 modes += "k";
                 
                }
            if (channel->getUserLimit() > 0) modes += "l";
            send_msg(c, RPL_CHANNELMODEIS(c.get_nick(), c.get_user(),c.get_hostname(),channelName,modes,""));
            send_msg(c, RPL_CREATIONTIME(c.get_nick(), c.get_user(),c.get_hostname(),channelName, oss.str()));
            // send_msg(c, RPL_UMODEIS(c.get_nick(), channelName, modes, c.get_nick()));
        } 
        else
        {
            send_msg(c, RPL_CHANNELMODEIS(c.get_nick(), c.get_user(),c.get_hostname(),channelName,"+",""));
            send_msg(c, RPL_CREATIONTIME(c.get_nick(), c.get_user(),c.get_hostname(),channelName, oss.str()));
        }
        // If no mode is specified, return the current modes
        return;
    }
    bool isOperator = false;
    const std::vector<Client*>& admins = channel->getAdmins();
    for (size_t i = 0; i < admins.size(); ++i) {
        if (admins[i] == &c) {
            isOperator = true;
            break;
        }
    }
      if (!isOperator) {
        send_msg(c, ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }
    std::string modeString = cmds[2];
    const std::vector<Client*>& users = channel->getUsers();
    bool adding = true;
    std::string modeChangeMsg = ":" + c.get_nick() + " MODE " + channelName + " ";
    for (size_t i = 0; i < modeString.length(); ++i) {
        char mode = modeString[i];
        if (mode == '+') {
            adding = true;
            continue;
        }
        if (mode == '-') {
            adding = false;
            continue;
        }
        switch (mode) {
            case 'i':
                if (adding) {
                    channel->setInviteOnly(true);
                    modeChangeMsg += "+i ";
                } else {
                    channel->setInviteOnly(false);
                    modeChangeMsg += "-i ";
                }
                break;
            case 't':
                if (adding) {
                    channel->setTopicRestriction(true);
                    modeChangeMsg += "+t ";
                } else {
                    channel->setTopicRestriction(false);
                    modeChangeMsg += "-t ";
                }
                break;
            case 'k':
                if (adding) {
                    if (cmds.size() < 4) {
                        send_msg(c, ERR_NEEDMODEPARM(channelName, "k"));
                        return;
                    }
                    std::string key = cmds[3];
                    channel->setKey(key);
                    modeChangeMsg += "+k " + key + " ";
                    cmds.erase(cmds.begin() + 3);
                } else {
                    channel->setKey("");
                    modeChangeMsg += "-k ";
                }
                break;
            case 'o': {
                if (cmds.size() < 4) {
                    send_msg(c, ERR_NEEDMOREPARAMS(std::string("MODE")));
                    return;
                }
                std::string targetNick = cmds[3];
                Client* targetClient = NULL;
                for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
                    if (it->second.get_nick() == targetNick) {
                        targetClient = &(it->second);
                        break;
                    }
                }
                if (!targetClient || !channel->hasClient(targetClient)) {
                    send_msg(c, ERR_USERNOTINCHANNEL(targetNick, channelName));
                    return;
                }
                if (adding) {
                    channel->addToAdmin(targetClient);
                    modeChangeMsg += "+o " + targetNick + " ";
                } else {
                    channel->removeFromAdmin(targetClient);
                    modeChangeMsg += "-o " + targetNick + " ";
                }
                cmds.erase(cmds.begin() + 3);
                break;
            }
            case 'l':
                if (adding) {
                    if (cmds.size() < 4) {
                        send_msg(c, ERR_NEEDMODEPARM(channelName, "l"));
                        return;
                    }
                    int limit = std::atoi(cmds[3].c_str());
                    if (limit <= 0) {
                        send_msg(c, ERR_INVALIDMODEPARM(channelName, "l"));
                        return;
                    }
                    if (users.size() > limit)
                    {
                        send_msg(c, "Number of users is too big to accept input.\r\n");
                        return;
                    }
                    channel->setUserLimit(limit);
                    modeChangeMsg += "+l " + cmds[3] + " ";
                    cmds.erase(cmds.begin() + 3);
                }
                else {
                    channel->setUserLimit(0);
                    modeChangeMsg += "-l ";
                }
                break;
            default:
                send_msg(c,":irc.com 472 " + c.get_nick() + " " + std::string(1, mode) + " :is unknown mode char to me");
                return;
        }
    }
    modeChangeMsg += "\r\n";
    for (size_t i = 0; i < users.size(); ++i) {
        send_msg(*users[i], modeChangeMsg);
    }
}
