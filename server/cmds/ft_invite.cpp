#include "../../headers/server.hpp"


void Server::ft_invite(std::vector<std::string> cmds, Client &c) {
    if (cmds.size() < 3) {
        send_msg(c, ERR_NEEDMOREPARAMS(std::string("INVITE")));
        return;
    }
    std::string targetNick = cmds[1];
    std::string channelName = cmds[2];
    if (channelName.empty() || (channelName[0] != '#')) {
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

    Client* targetClient = NULL;
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.get_nick() == targetNick) {
            targetClient = &(it->second);
            break;
        }
    }
    if (!targetClient) {
        send_msg(c, ERR_NOSUCHNICK(targetNick));
        return;
    }
    if (channel) {
        if (!channel->hasClient(&c)) {
            send_msg(c, ERR_NOTONCHANNEL(c.get_nick(), channelName));
            return;
        }
        if (channel->hasClient(targetClient)) {
            send_msg(c, ERR_USERONCHANNEL(targetNick, channelName));
            return;
        }
        if (channel->isInviteOnly()) {
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
        }
    }
    if (channel) {
        channel->invite(targetClient);
    }
    std::string inviteMsg = ":" + c.get_nick() + " INVITE " + targetNick + " " + channelName + "\r\n";
    send_msg(*targetClient, inviteMsg);
    send_msg(c, RPL_INVITING(c.get_nick(), targetNick, channelName));
}