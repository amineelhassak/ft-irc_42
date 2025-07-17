#include "../headers/server.hpp"
#include "../headers/client.hpp"

std::string	get_current_time()
{
	char	buff[100];
	std::time_t now = std::time(NULL);
	std::tm* time_info = std::localtime(&now);
	std::strftime(buff, sizeof(buff), "%a %b %d %Y %H:%M:%S", time_info);
	return (std::string(buff));
}

void	Server::send_msg(Client& c, std::string msg)
{
	std::string	message;

	message = ":" + this->name + " " + msg + "\r\n";
	send(c.get_fd(), message.c_str(), message.length(), 0);
}

void	Server::welcome_msg(Client& c)
{
	std::string nick = c.get_nick();
	std::string	user = c.get_user();
	std::string name = this->name;
	std::string	version = "1.0";
	std::string date = get_current_time();
	std::string	user_modes = "i";
	std::string	chan_modes = "it";
	send_msg(c, "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + name);
	send_msg(c, "002 " + nick + " :Your host is " + name + ", running version " + version);
	send_msg(c, "003 " + nick + " :This server was created " + date);
	send_msg(c, "004 " + nick + " " + name + " " + version + " " + user_modes + " " + chan_modes);
}



void	Server::handle_line(Client& c, std::vector<std::string> cmd)
{
	// std::stringstream	ss;
	// ss << line;
	// std::string	cmd;
	// ss >> cmd;
;
	if (cmd.size() == 2 && cmd[0] == "PASS")
	{
	// std::cout << "ah ana hna "  << std::endl;J
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
	else if (cmd.size() == 2 && cmd[0] == "NICK")
	{
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

std::vector<std::string> split(const std::string& input) {
	std::istringstream iss(input);
	std::vector<std::string> result;
	std::string word;
	while (iss >> word)
		result.push_back(word);
	return result;
}

std::vector<std::string> splitByComma(const std::string &input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

int	Server::existChannel(std::string name)
{
	// for(int i = 0;i < allChannels.size();i++)
	// {
	// 	if (allChannels[i].getName() == name)
	// 		return 1;
	// }
	return 0;
}




void Server::ft_join(std::vector<std::string> cmds, Server *server, Client *c)
{
    if (cmds.size() < 2) {
        send_msg(*c, "461 JOIN :Not enough parameters"); // ERR_NEEDMOREPARAMS
        return;
    }
    std::vector<std::string> channels = splitByComma(cmds[1]);
    std::vector<std::string> passwords;
    if (cmds.size() > 2)
        passwords = splitByComma(cmds[2]);
    else
        passwords.resize(channels.size());

    for (size_t i = 0; i < channels.size(); ++i) {
        const std::string &name = channels[i];
        std::string key = (i < passwords.size()) ? passwords[i] : "";
        if (name.empty() || (name[0] != '#' && name[0] != '&')) {
            send_msg(*c, "476 " + name + " :Bad Channel Mask"); // ERR_BADCHANMASK
            continue;
        }
        // Find or create channel
        Channel* cl = nullptr;
        for (size_t j = 0; j < allChannels.size(); ++j) {
            if (allChannels[j].getName() == name) {
                cl = &allChannels[j];
                break;
            }
        }
        if (!cl) {
            allChannels.push_back(Channel(name));
            cl = &allChannels.back();
            cl->addToAdmin(c);
            cl->addToChannel(c);
        }
		else {
			if (cl->hasClient(c))
			{
				send_msg(*c, "443 " + c->get_nick() + " " + name + " :is already on channel"); // ERR_USERONCHANNEL
				continue;
			}
			// Check invite-only
			if (cl->isInviteOnly() && !cl->isInvited(c)) {
				send_msg(*c, "473 " + name + " :Cannot join channel (+i)"); // ERR_INVITEONLYCHAN
				continue;
			}
			// Check key
			if (!cl->getKey().empty() && cl->getKey() != key) {
				send_msg(*c, "475 " + name + " :Cannot join channel (+k)"); // ERR_BADCHANNELKEY
				continue;
			}
			// Check user limit
			if (cl->getUserLimit() > 0 && (int)cl->userCount() >= cl->getUserLimit()) {
				send_msg(*c, "471 " + name + " :Cannot join channel (+l)"); // ERR_CHANNELISFULL
				continue;
			}
			// Add user to channel
			cl->addToChannel(c);
			send_msg(*c, ":" + c->get_nick() + " JOIN " + name);

		}
        // Send JOIN reply
        // // Send topic
        // if (!cl->getTopic().empty())
        //     send_msg(*c, "332 " + c->get_nick() + " " + name + " :" + cl->getTopic()); // RPL_TOPIC
        // else
        //     send_msg(*c, "331 " + c->get_nick() + " " + name + " :No topic is set"); // RPL_NOTOPIC
        // // Send names
        // std::string namesReply = "353 " + c->get_nick() + " = " + name + " :";
        // const std::vector<Client*>& users = cl->getUsers();
        // const std::vector<Client*>& admins = cl->getAdmins();
        // for (size_t ui = 0; ui < users.size(); ++ui) {
        //     Client* user = users[ui];
        //     bool isOp = false;
        //     for (size_t ai = 0; ai < admins.size(); ++ai) {
        //         if (admins[ai] == user) { isOp = true; break; }
        //     }
        //     if (isOp)
        //         namesReply += "@";
        //     namesReply += user->get_nick() + " ";
        // }
        // send_msg(*c, namesReply);
        // send_msg(*c, "366 " + c->get_nick() + " " + name + " :End of /NAMES list"); // RPL_ENDOFNAMES
    }
}

// MODE #amine +lkoil 10 amine amine 77
void Server::ft_mode(std::vector<std::string> cmds, Server* server, Client* c) {
    // Vérifie s'il y a assez de paramètres (au moins le nom du canal)
    if (cmds.size() < 2) {
        send_msg(*c, ERR_NEEDMOREPARAMS(std::string("MODE")));
        return;
    }

    std::string channelName = cmds[1];
    
    // Vérifie que le nom du canal commence par # ou &
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        send_msg(*c, ERR_BADCHANMASK(channelName));
        return;
    }
    
    // Recherche le canal dans la liste des canaux existants
    Channel* channel = nullptr;
    for (size_t i = 0; i < allChannels.size(); ++i) {
        if (allChannels[i].getName() == channelName) {
            channel = &allChannels[i];
            break;
        }
    }
    
    // Vérifie si le canal existe
    if (!channel) {
        send_msg(*c, ERR_NOSUCHCHANNEL(channelName));
        return;
    }

    // Vérifie si l'utilisateur est sur le canal
    if (!channel->hasClient(c)) {
        send_msg(*c, ERR_NOTONCHANNEL(c->get_nick(), channelName));
        return;
    }

    // Si aucun mode n'est spécifié, retourne (pourrait afficher les modes actuels)
    if (cmds.size() < 3) {
        return;
    }

    std::string modeString = cmds[2];
    bool adding = true; // true pour +, false pour -
    std::string modeChangeMsg = ":" + c->get_nick() + " MODE " + channelName + " ";
    
    // Parse la chaîne de modes (ex: "+i", "-k", "+l 10", etc.)
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
                        send_msg(*c, ERR_NEEDMODEPARM(channelName, "k"));
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
                    send_msg(*c, ERR_NEEDMOREPARAMS(std::string("MODE")));
                    return;
                }
                std::string targetNick = cmds[3];
                
                // Recherche le client cible dans la liste des clients
                Client* targetClient = nullptr;
                for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
                    if (it->second.get_nick() == targetNick) {
                        targetClient = &(it->second);
                        break;
                    }
                }
                
                // Vérifie que l'utilisateur cible existe et est sur le canal
                if (!targetClient || !channel->hasClient(targetClient)) {
                    send_msg(*c, ERR_USERNOTINCHANNEL(targetNick, channelName));
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
                        send_msg(*c, ERR_NEEDMODEPARM(channelName, "l"));
                        return;
                    }
                    int limit = std::atoi(cmds[3].c_str());
                    // Vérifie que la limite est valide (positive)
                    if (limit <= 0) {
                        send_msg(*c, ERR_INVALIDMODEPARM(channelName, "l"));
                        return;
                    }
                    channel->setUserLimit(limit);
                    modeChangeMsg += "+l " + cmds[3] + " ";
                    // Supprime le paramètre utilisé
                    cmds.erase(cmds.begin() + 3);
                } else {
                    channel->setUserLimit(0);
                    modeChangeMsg += "-l ";
                }
                break;

            default:
                // Mode inconnu
                send_msg(*c, ERR_UNKNOWNMODE(c->get_nick(), channelName, std::string(1, mode)));
                return;
        }
    }

    // Envoie le message de changement de mode à tous les utilisateurs du canal
    const std::vector<Client*>& users = channel->getUsers();
    for (size_t i = 0; i < users.size(); ++i) {
        send_msg(*users[i], modeChangeMsg);
    }
}

// Fonction pour expulser un utilisateur d'un canal
void Server::ft_kick(std::vector<std::string> cmds, Server* server, Client* c) {
    // Vérifie s'il y a assez de paramètres (canal et utilisateur à expulser)
    if (cmds.size() < 3) {
        send_msg(*c, ERR_NEEDMOREPARAMS(std::string("KICK")));
        return;
    }

    std::string channelName = cmds[1];
    std::string targetNick = cmds[2];
    std::string reason = (cmds.size() > 3) ? cmds[3] : c->get_nick(); // Raison optionnelle

    // Vérifie que le nom du canal commence par # ou &
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        send_msg(*c, ERR_BADCHANMASK(channelName));
        return;
    }

    // Recherche le canal
    Channel* channel = nullptr;
    for (size_t i = 0; i < allChannels.size(); ++i) {
        if (allChannels[i].getName() == channelName) {
            channel = &allChannels[i];
            break;
        }
    }

    // Vérifie si le canal existe
    if (!channel) {
        send_msg(*c, ERR_NOSUCHCHANNEL(channelName));
        return;
    }

    // Vérifie si l'utilisateur qui kick est sur le canal
    if (!channel->hasClient(c)) {
        send_msg(*c, ERR_NOTONCHANNEL(c->get_nick(), channelName));
        return;
    }

    // Vérifie si l'utilisateur qui kick est opérateur du canal
    bool isOperator = false;
    const std::vector<Client*>& admins = channel->getAdmins();
    for (size_t i = 0; i < admins.size(); ++i) {
        if (admins[i] == c) {
            isOperator = true;
            break;
        }
    }

    if (!isOperator) {
        send_msg(*c, ERR_CHANOPRIVSNEEDED(channelName));
        return;
    }

    // Recherche l'utilisateur cible
    Client* targetClient = nullptr;
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.get_nick() == targetNick) {
            targetClient = &(it->second);
            break;
        }
    }

    // Vérifie si l'utilisateur cible existe
    if (!targetClient) {
        send_msg(*c, ERR_NOSUCHNICK(targetNick));
        return;
    }

    // Vérifie si l'utilisateur cible est sur le canal
    if (!channel->hasClient(targetClient)) {
        send_msg(*c, ERR_USERNOTINCHANNEL(targetNick, channelName));
        return;
    }

    // Retire l'utilisateur du canal
    channel->removeFromChannel(targetClient);

    // Envoie le message de kick à tous les utilisateurs du canal
    std::string kickMsg = ":" + c->get_nick() + " KICK " + channelName + " " + targetNick + " :" + reason;
    const std::vector<Client*>& users = channel->getUsers();
    for (size_t i = 0; i < users.size(); ++i) {
        send_msg(*users[i], kickMsg);
    }

    // Envoie aussi le message à l'utilisateur expulsé
    send_msg(*targetClient, kickMsg);
}

// Fonction pour inviter un utilisateur sur un canal
void Server::ft_invite(std::vector<std::string> cmds, Server* server, Client* c) {
    // Vérifie s'il y a assez de paramètres (utilisateur et canal)
    if (cmds.size() < 3) {
        send_msg(*c, ERR_NEEDMOREPARAMS(std::string("INVITE")));
        return;
    }

    std::string targetNick = cmds[1];
    std::string channelName = cmds[2];

    // Vérifie que le nom du canal commence par # ou &
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        send_msg(*c, ERR_BADCHANMASK(channelName));
        return;
    }

    // Recherche le canal
    Channel* channel = nullptr;
    for (size_t i = 0; i < allChannels.size(); ++i) {
        if (allChannels[i].getName() == channelName) {
            channel = &allChannels[i];
            break;
        }
    }

    // Recherche l'utilisateur cible
    Client* targetClient = nullptr;
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->second.get_nick() == targetNick) {
            targetClient = &(it->second);
            break;
        }
    }

    // Vérifie si l'utilisateur cible existe
    if (!targetClient) {
        send_msg(*c, ERR_NOSUCHNICK(targetNick));
        return;
    }

    // Si le canal existe, vérifie les permissions
    if (channel) {
        // Vérifie si l'utilisateur qui invite est sur le canal
        if (!channel->hasClient(c)) {
            send_msg(*c, ERR_NOTONCHANNEL(c->get_nick(), channelName));
            return;
        }

        // Vérifie si l'utilisateur cible est déjà sur le canal
        if (channel->hasClient(targetClient)) {
            send_msg(*c, ERR_USERONCHANNEL(targetNick, channelName));
            return;
        }

        // Si le canal est en mode invite-only, vérifie que l'inviteur est opérateur
        if (channel->isInviteOnly()) {
            bool isOperator = false;
            const std::vector<Client*>& admins = channel->getAdmins();
            for (size_t i = 0; i < admins.size(); ++i) {
                if (admins[i] == c) {
                    isOperator = true;
                    break;
                }
            }
            
            if (!isOperator) {
                send_msg(*c, ERR_CHANOPRIVSNEEDED(channelName));
                return;
            }
        }
    }

    // Ajoute l'invitation si le canal existe
    if (channel) {
        channel->invite(targetClient);
    }

    // Envoie le message d'invitation à l'utilisateur cible
    std::string inviteMsg = ":" + c->get_nick() + " INVITE " + targetNick + " " + channelName;
    send_msg(*targetClient, inviteMsg);

    // Envoie la confirmation à l'utilisateur qui invite
    send_msg(*c, RPL_INVITING(c->get_nick(), targetNick, channelName));

    // TODO: Si l'utilisateur cible est away, envoyer RPL_AWAY
    // if (targetClient->isAway()) {
    //     Messages::send_msg(*c, RPL_AWAY(targetNick, targetClient->getAwayMessage()));
    // }
}

// Fonction pour gérer le sujet (topic) d'un canal
void Server::ft_topic(std::vector<std::string> cmds, Server* server, Client* c) {
    // Vérifie s'il y a assez de paramètres (au moins le nom du canal)
    if (cmds.size() < 2) {
        send_msg(*c, ERR_NEEDMOREPARAMS(std::string("TOPIC")));
        return;
    }

    std::string channelName = cmds[1];
    
    // Vérifie que le nom du canal commence par # ou &
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        send_msg(*c, ERR_BADCHANMASK(channelName));
        return;
    }
    
    // Recherche le canal
    Channel* channel = nullptr;
    for (size_t i = 0; i < allChannels.size(); ++i) {
        if (allChannels[i].getName() == channelName) {
            channel = &allChannels[i];
            break;
        }
    }

    // Vérifie si le canal existe
    if (!channel) {
        send_msg(*c, ERR_NOSUCHCHANNEL(channelName));
        return;
    }

    // Vérifie si l'utilisateur est sur le canal
    if (!channel->hasClient(c)) {
        send_msg(*c, ERR_NOTONCHANNEL(c->get_nick(), channelName));
        return;
    }

    // Si aucun topic n'est fourni, affiche le topic actuel
    if (cmds.size() < 3) {
        std::string currentTopic = channel->getTopic();
        if (currentTopic.empty()) {
            send_msg(*c, RPL_NOTOPIC(c->get_nick(), channelName));
        } else {
            send_msg(*c, RPL_TOPIC(c->get_nick(), channelName, currentTopic));
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
            if (admins[i] == c) {
                isOperator = true;
                break;
            }
        }
        
        if (!isOperator) {
            send_msg(*c, ERR_CHANOPRIVSNEEDED(channelName));
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
    std::string topicMsg = ":" + c->get_nick() + " TOPIC " + channelName + " :" + newTopic;
    const std::vector<Client*>& users = channel->getUsers();
    for (size_t i = 0; i < users.size(); ++i) {
        send_msg(*users[i], topicMsg);
    }
}

// Fonction pour gérer les messages privés (PRIVMSG)
void Server::ft_privmsg(std::vector<std::string> cmds, Server* server, Client* c) {
    // Vérifie s'il y a assez de paramètres (destinataire et message)
    if (cmds.size() < 2) {
        send_msg(*c, ERR_NORECIPIENT(std::string("PRIVMSG")));
        return;
    }

    // Vérifie s'il y a un message à envoyer
    if (cmds.size() < 3) {
        send_msg(*c, ERR_NOTEXTTOSEND());
        return;
    }

    std::string receivers = cmds[1];
    // Construit le message (tous les paramètres après le destinataire)
    std::string message;
    for (size_t i = 2; i < cmds.size(); ++i) {
        if (i > 2) message += " ";
        message += cmds[i];
    }
    // Supprime les deux points au début si présents
    if (message.length() > 0 && message[0] == ':') {
        message = message.substr(1);
    }
    // Vérifie que le message n'est pas vide
    if (message.empty()) {
        send_msg(*c, ERR_NOTEXTTOSEND());
        return;
    }
    // Sépare les destinataires par virgules
    std::vector<std::string> receiverList = splitByComma(receivers);
    // Traite chaque destinataire
    for (size_t i = 0; i < receiverList.size(); ++i) {
        std::string receiver = receiverList[i];
        // Vérifie si c'est un canal (commence par # ou &)
        if (receiver[0] == '#' || receiver[0] == '&') {
            // Message vers un canal
            Channel* channel = nullptr;
            for (size_t j = 0; j < allChannels.size(); ++j) {
                if (allChannels[j].getName() == receiver) {
                    channel = &allChannels[j];
                    break;
                }
            }
            if (!channel) {
                send_msg(*c, ERR_NOSUCHCHANNEL(receiver));
                continue;
            }
            // Vérifie si l'utilisateur est sur le canal
            if (!channel->hasClient(c)) {
                send_msg(*c, ERR_CANNOTSENDTOCHAN(receiver));
                continue;
            }
            // Envoie le message à tous les utilisateurs du canal
            std::string privmsgMsg = ":" + c->get_nick() + " PRIVMSG " + receiver + " :" + message;
            const std::vector<Client*>& users = channel->getUsers();
            for (size_t k = 0; k < users.size(); ++k) {
                if (users[k] != c) // N'envoie pas le message à l'expéditeur
                    send_msg(*users[k], privmsgMsg);
            }
        } else {
            // Message vers un utilisateur
            Client* targetClient = nullptr;
            for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
                if (it->second.get_nick() == receiver) {
                    targetClient = &(it->second);
                    break;
                }
            }
            if (!targetClient) {
                send_msg(*c, ERR_NOSUCHNICK(receiver));
                continue;
            }
            // Envoie le message privé à l'utilisateur
            std::string privmsgMsg = ":" + c->get_nick() + " PRIVMSG " + receiver + " :" + message;
            send_msg(*targetClient, privmsgMsg);
            // TODO: Si l'utilisateur est away, envoyer RPL_AWAY
            // if (targetClient->isAway()) {
            //     send_msg(*c, RPL_AWAY(receiver, targetClient->getAwayMessage()));
            // }
        }
    }
}

void Server::cmds(std::vector<std::string> cmds, Server* server, Client* c) {
	std::string keyword = cmds[0];
    // Dispatch vers les commandes IRC principales
    if (keyword == "JOIN") {
        ft_join(cmds, server, c);
    }
    else if (keyword == "MODE") {
        ft_mode(cmds, server, c);
    }
    else if (keyword == "INVITE")
	{
        ft_invite(cmds,server,c);
    }
    else if (keyword == "KICK") {
        ft_kick(cmds,server,c);
    }
    else if (keyword == "PRIVMSG") {
        ft_privmsg(cmds,server,c);
    }
    else if (keyword == "TOPIC") {
        ft_topic(cmds,server,c);
    }
    else {
        
    }
}




void	Server::handle_buff_line(Client& c, const std::string& buff)
{
	
	c.buffer += buff;
	std::vector<std::string> cmd = split(buff);

	// Debug: print received command
	std::cout << "Received command: ";
	for (size_t i = 0; i < cmd.size(); ++i) {
		std::cout << "[" << cmd[i] << "] ";
	}
	std::cout << std::endl;

	handle_line(c,cmd);
	cmds(cmd, this, &c);
}


void    Server::init_socket()
{
	struct	sockaddr_in server;

	this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->socket_fd == -1)
	{
		std::cerr << "socket failed!" << std::endl;
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->socket_fd, (struct sockaddr*)&server, sizeof(server)) != 0)
	{
		std::cerr << "bind failed!" << std::endl;
		exit(1);
	}
	if (listen(this->socket_fd, SOMAXCONN) != 0)
	{
		std::cerr << "listen failed!" << std::endl;
		exit(1);
	}
	struct pollfd listener;
	listener.fd = socket_fd;
	listener.events = POLLIN;
	poll_fds.push_back(listener);
	while (true)
	{
		int res;
		res = poll(poll_fds.data(), poll_fds.size(), -1);
		if (res > 0)
		{
			if (poll_fds[0].revents & POLLIN)
			{
				int fd_client;
				fd_client = accept(socket_fd, NULL, NULL);
				if (fd_client < 0)
				{
					std::cerr << "accept failed!" << std::endl;
					continue;
				}
				else
				{
					clients[fd_client] = Client(fd_client);
					pollfd	new_client;
					new_client.fd = fd_client;
					new_client.events = POLLIN;
					poll_fds.push_back(new_client); //the new client have just added
					std::cout << "A new client just connected!" << std::endl;
				}
			}
			for (long unsigned int i = 1; i < poll_fds.size(); ++i)
			{
				if (poll_fds[i].revents & POLLIN)
				{
					char buff[512];
					ssize_t read_size;
					read_size = recv(poll_fds[i].fd, buff, sizeof(buff), 0);
					if (read_size <= 0)
					{
						close(poll_fds[i].fd);
						poll_fds.erase(poll_fds.begin() + i);
						--i;
						continue;
					}
					buff[read_size] = '\0';
					
					// std::cout << "buff2 : ";
					// std::cout << "buff : " << buff << std::endl ;
					// std::cout << "buff3 : ";
					// std::cout.flush();
					handle_buff_line(clients[poll_fds[i].fd], buff);
				}
				else if (poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
				{
					if (poll_fds[i].revents & POLLHUP)
						std::cerr << "client disconnected (POLLHUP)!" << std::endl;
					if (poll_fds[i].revents & POLLERR)
						std::cerr << "client socket error (POLLERR)!" << std::endl;
					if (poll_fds[i].revents & POLLNVAL)
						std::cerr << "client invalid fd (POLLNVAL)!" << std::endl;
					close(poll_fds[i].fd);
					poll_fds.erase(poll_fds.begin() + i);
					--i;
					continue;
				}
			}
		}
	}
}

