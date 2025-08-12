#include "../../headers/server.hpp"


void Server::ft_topic(std::vector<std::string> cmds, Client &c) {
    if (cmds.size() < 2) {
        send_msg(c, ERR_NEEDMOREPARAMS(std::string("TOPIC")));
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
    // Si aucun topic n'est fourni, affiche le topic actuel
    if (cmds.size() < 3) {
        std::string currentTopic = channel->getTopic();
        if (currentTopic.empty()) {
            send_msg(c, RPL_NOTOPIC(c.get_nick(), channelName));
        } else {
            send_msg(c, RPL_TOPIC(c.get_nick(), channelName, currentTopic));
        }
        return;
    }
    if(!channel->isTopicRestricted())
    {
        send_msg(c, "u cannot change topic on this channel\r\n");
        return;
    }
    std::string newTopic;
    for (size_t i = 2; i < cmds.size(); ++i) {
        if (i > 2) newTopic += " ";
        newTopic += cmds[i];
    }
    if (newTopic.length() > 0 && newTopic[0] == ':') {
        newTopic = newTopic.substr(1);
    }
    if (channel->isTopicRestricted()) {
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
    if (newTopic.empty() && cmds.size() >= 3 && cmds[2] == ":") {
        newTopic = "";
    }
    channel->setTopic(newTopic);
    std::string topicMsg = ":" + c.get_nick() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    const std::vector<Client*>& users = channel->getUsers();
    for (size_t i = 0; i < users.size(); ++i) {
        send_msg(*users[i], topicMsg);
    }
}