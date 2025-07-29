#include "../../headers/server.hpp"

void Server::ft_privmsg(std::vector<std::string> cmds, Server* server, Client& c)
{
    if (cmds.size() < 2) {
        send_msg(c, ERR_NORECIPIENT(std::string("PRIVMSG")));
        return;
    }
    if (cmds.size() < 3) {
        send_msg(c, ERR_NOTEXTTOSEND());
        return;
    }
    std::string receivers = cmds[1];
    std::string message;
    for (size_t i = 2; i < cmds.size(); ++i) {
        if (i > 2) message += " ";
        message += cmds[i];
    }
    if (message.length() > 0 && message[0] == ':') {
        message = message.substr(1);
    }
    if (message.empty()) {
        send_msg(c, ERR_NOTEXTTOSEND());
        return;
    }
    std::vector<std::string> receiverList = splitByComma(receivers);
    for (size_t i = 0; i < receiverList.size(); ++i) {
        std::string receiver = receiverList[i];
        if (receiver[0] == '#' ) {
            Channel* channel = NULL;
            for (size_t j = 0; j < allChannels.size(); ++j) {
                if (allChannels[j].getName() == receiver) {
                    channel = &allChannels[j];
                    break;
                }
            }
            if (!channel) {
                send_msg(c, ERR_NOSUCHCHANNEL(receiver));
                continue;
            }
            if (!channel->hasClient(&c)) {
                send_msg(c, ERR_CANNOTSENDTOCHAN(receiver));
                continue;
            }
            std::string privmsgMsg = ":" + c.get_nick() + "!" + c.get_user() + "@" + c.get_hostname() + " PRIVMSG " + receiver + " :" + message + "\r\n";
            const std::vector<Client*>& users = channel->getUsers();
            for (size_t k = 0; k < users.size(); ++k) {
                if (users[k] != &c) 
                    send_msg(*users[k], privmsgMsg);
            }
        } else {
            Client* targetClient = NULL;
            for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
                if (it->second.get_nick() == receiver) {
                    targetClient = &(it->second);
                    break;
                }
            }
            if (!targetClient) {
                send_msg(c, ERR_NOSUCHNICK(receiver));
                continue;
            }
            std::string privmsgMsg = ":" + c.get_nick() + "!" + c.get_user() + "@" + c.get_hostname() + " PRIVMSG " + receiver + " :" + message + "\r\n";
            send_msg(*targetClient, privmsgMsg);
        }
    }
}
