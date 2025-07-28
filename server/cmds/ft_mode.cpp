#include "../../headers/server.hpp"

void Server::ft_mode(std::vector<std::string> cmds, Server* server, Client& c) {
    if (cmds.size() < 2) {
        send_msg(c, ERR_NEEDMOREPARAMS(std::string("MODE")));
        return;
    }

    std::string channelName = cmds[1];
    // Vérifie que le nom du canal commence par # ou &
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        send_msg(c, ERR_BADCHANMASK(channelName));
        return;
    }
    // Recherche le canal dans la liste des canaux existants
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
    // Si aucun mode n'est spécifié, retourne (pourrait afficher les modes actuels)
    if (cmds.size() < 3) {
        return;
    }
    std::string modeString = cmds[2];
    const std::vector<Client*>& users = channel->getUsers();
    bool adding = true; // true pour +, false pour -
    std::string modeChangeMsg = ":" + c.get_nick() + " MODE " + channelName + " ";
    // Parcours la chaîne de modes (ex: +i, -k, +l 10, etc.)
    for (size_t i = 0; i < modeString.length(); ++i) {
        char mode = modeString[i];
        // Détermine si on ajoute (+) ou retire (-) le mode
        if (mode == '+') {
            adding = true;
            continue;
        }
        if (mode == '-') {
            adding = false;
            continue;
        }
        // Traite chaque mode individuellement
        switch (mode) {
            case 'i': // Mode invite-only (invitation uniquement)
                if (adding) {
                    channel->setInviteOnly(true);
                    modeChangeMsg += "+i ";
                } else {
                    channel->setInviteOnly(false);
                    modeChangeMsg += "-i ";
                }
                break;
            case 't': // Restriction du topic (seuls les opérateurs peuvent changer le sujet)
                if (adding) {
                    channel->setTopicRestriction(true);
                    modeChangeMsg += "+t ";
                } else {
                    channel->setTopicRestriction(false);
                    modeChangeMsg += "-t ";
                }
                break;
            case 'k': // Clé du canal (mot de passe)
                if (adding) {
                    // Vérifie qu'un paramètre (la clé) est fourni
                    if (cmds.size() < 4) {
                        send_msg(c, ERR_NEEDMODEPARM(channelName, "k"));
                        return;
                    }
                    std::string key = cmds[3];
                    channel->setKey(key);
                    modeChangeMsg += "+k " + key + " ";
                    // Supprime le paramètre utilisé
                    cmds.erase(cmds.begin() + 3);
                } else {
                    channel->setKey("");
                    modeChangeMsg += "-k ";
                }
                break;
            case 'o': { // Privilège d'opérateur
                // Vérifie qu'un paramètre (le nickname) est fourni
                if (cmds.size() < 4) {
                    send_msg(c, ERR_NEEDMOREPARAMS(std::string("MODE")));
                    return;
                }
                std::string targetNick = cmds[3];
                // Recherche le client cible dans la liste des clients
                Client* targetClient = NULL;
                for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
                    if (it->second.get_nick() == targetNick) {
                        targetClient = &(it->second);
                        break;
                    }
                }
                // Vérifie que l'utilisateur cible existe et est sur le canal
                if (!targetClient || !channel->hasClient(targetClient)) {
                    send_msg(c, ERR_USERNOTINCHANNEL(targetNick, channelName));
                    return;
                }
                if (adding) {
                    channel->addToAdmin(targetClient);
                    modeChangeMsg += "+o " + targetNick + " ";
                } else {
                    // Retire des admins (il faudrait ajouter removeFromAdmin à la classe Channel)
                    // Pour l'instant, on reconnaît juste le changement
                    modeChangeMsg += "-o " + targetNick + " ";
                }
                // Supprime le paramètre utilisé
                cmds.erase(cmds.begin() + 3);
                break;
            }
            case 'l': // Limite d'utilisateurs
                if (adding) {
                    // Vérifie qu'un paramètre (la limite) est fourni
                    if (cmds.size() < 4) {
                        send_msg(c, ERR_NEEDMODEPARM(channelName, "l"));
                        return;
                    }
                    int limit = std::atoi(cmds[3].c_str());
                    // Vérifie que la limite est valide (positive)
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
                    // Supprime le paramètre utilisé
                    cmds.erase(cmds.begin() + 3);
                }
                else {
                    channel->setUserLimit(0);
                    modeChangeMsg += "-l ";
                }
                break;
            default:
                // Mode inconnu
                send_msg(c, ERR_UNKNOWNMODE(c.get_nick(), channelName, std::string(1, mode)));
                return;
        }
    }
    modeChangeMsg += "\r\n";
    // Envoie le message de changement de mode à tous les utilisateurs du canal
    for (size_t i = 0; i < users.size(); ++i) {
        send_msg(*users[i], modeChangeMsg);
    }
}
