#include "../../headers/server.hpp"


void Server::ft_topic(std::vector<std::string> cmds, Server* server, Client &c) {
    // Vérifie s'il y a assez de paramètres (au moins le nom du canal)
    if (cmds.size() < 2) {
        send_msg(c, ERR_NEEDMOREPARAMS(std::string("TOPIC")));
        return;
    }
    std::string channelName = cmds[1];
    // Vérifie que le nom du canal commence par # ou &
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        send_msg(c, ERR_BADCHANMASK(channelName));
        return;
    }
    // Recherche le canal
    Channel* channel = NULL;
    for (size_t i = 0; i < allChannels.size(); ++i) {
        if (allChannels[i].getName() == channelName) {
            channel = &allChannels[i];
            break;
        }
    }
    // Vérifie si le canal existe
    if (!channel) {
        send_msg(c, ERR_NOSUCHCHANNEL(channelName));
        return;
    }
    // Vérifie si l'utilisateur est sur le canal
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
    // Construit le nouveau topic (tous les paramètres après le nom du canal)
    std::string newTopic;
    for (size_t i = 2; i < cmds.size(); ++i) {
        if (i > 2) newTopic += " ";
        newTopic += cmds[i];
    }
    // Supprime les deux points au début si présents
    if (newTopic.length() > 0 && newTopic[0] == ':') {
        newTopic = newTopic.substr(1);
    }
    // Vérifie si le canal est en mode +t (topic restriction)
    // Si oui, seuls les opérateurs peuvent changer le topic
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
    // Si le topic est vide (juste ":"), on le traite comme un topic vide
    if (newTopic.empty() && cmds.size() >= 3 && cmds[2] == ":") {
        newTopic = "";
    }
    // Change le topic du canal
    channel->setTopic(newTopic);
    // Envoie le message de changement de topic à tous les utilisateurs du canal
    std::string topicMsg = ":" + c.get_nick() + " TOPIC " + channelName + " :" + newTopic;
    const std::vector<Client*>& users = channel->getUsers();
    for (size_t i = 0; i < users.size(); ++i) {
        send_msg(*users[i], topicMsg);
    }
}