#include "../../headers/server.hpp"

// void Server::checkAdmin(Channel *c)
// {
//       if (c->size() == 0)
//         c->admins->push_back(c->users[0])
// }

void Server::ft_kick(std::vector<std::string> cmds, Server* server, Client& c) {
    if (cmds.size() < 3) {
        send_msg(c, ERR_NEEDMOREPARAMS(std::string("KICK")));
        return;
    }
    std::string channelName = cmds[1];
    std::string targetNick = cmds[2];
    std::string reason = (cmds.size() > 3) ? cmds[3] : c.get_nick(); 
    
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
    const std::vector<Client*>& users = channel->getUsers();
     
    bool isOperator = false;
    const std::vector<Client*>& admins = channel->getAdmins();
    for (size_t i = 0; i < admins.size(); ++i) {
        if (admins[i] == &c) {
            isOperator = true;
            break;
        }
    }
    if (users.size() == 1)
    {
        send_msg(c, "YOU CAN'T KICK\r\n");
        return ;
    }
    if (!isOperator) {
        send_msg(c, ERR_CHANOPRIVSNEEDED(channelName));
        return;
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
    if (!channel->hasClient(targetClient)) {
        send_msg(c, ERR_USERNOTINCHANNEL(targetNick, channelName));
        return;
    }
    channel->removeFromChannel(targetClient);
    std::string kickMsg = ":" + c.get_nick() + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    for (size_t i = 0; i < users.size(); ++i) {
        send_msg(*users[i], kickMsg);
    }
    send_msg(*targetClient, kickMsg);
}