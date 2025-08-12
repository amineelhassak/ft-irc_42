#include "../headers/channel.hpp"


Channel::Channel(const std::string &name)
    : name(name), topic(""), key(""), userLimit(0), inviteOnly(false), topicRestriction(true) {
    creationTime = time(NULL);
    }

// Récupère le nom du canal
std::string Channel::getName() const { return name; }
// Récupère le sujet du canal
std::string Channel::getTopic() const { return topic; }
// Définit le sujet du canal
void Channel::setTopic(const std::string& t) { topic = t; }

// Ajoute un opérateur (admin) au canal
// ... existing code ...
void Channel::addToAdmin(Client* c) {
    // Vérifie si l'utilisateur est déjà admin
    bool dejaAdmin = false;
    for (size_t i = 0; i < admins.size(); ++i) {
        if (admins[i] == c) {
            dejaAdmin = true;
            break;
        }
    }
    if (!dejaAdmin)
        admins.push_back(c);
}
// ... existing code ...

// Ajoute un utilisateur au canal
void Channel::addToChannel(Client* c) {
    if (!hasClient(c))
        users.push_back(c);
}

// Retire un utilisateur du canal (et des admins si besoin)
void Channel::removeFromChannel(Client* c) {
    users.erase(std::remove(users.begin(), users.end(), c), users.end());
    admins.erase(std::remove(admins.begin(), admins.end(), c), admins.end());
    invites.erase(c);
}

// Vérifie si un utilisateur est déjà sur le canal
bool Channel::hasClient(Client* c) const {
    return std::find(users.begin(), users.end(), c) != users.end();
}

// Invite un utilisateur
void Channel::invite(Client* c) {
    invites.insert(c);
}
// Vérifie si un utilisateur est invité
bool Channel::isInvited(Client* c) const {
    for (std::set<Client*>::const_iterator it = invites.begin(); it != invites.end(); ++it) {
        if (*it == c)
            return true;
    }
    return false;
}

// Active/désactive le mode "invite-only"
void Channel::setInviteOnly(bool b)
{ 
    inviteOnly = b; 
}
// Vérifie si le canal est en mode "invite-only"
bool Channel::isInviteOnly() const {
    return inviteOnly;
}
// Définit la clé du canal
void Channel::setKey(const std::string& k) {
    key = k;
}
// Récupère la clé du canal
std::string Channel::getKey() const
{
    return key;
}
// Définit la limite d'utilisateurs
void Channel::setUserLimit(int limit) {
    userLimit = limit;
}
// Récupère la limite d'utilisateurs
int Channel::getUserLimit() const {
    return userLimit;
}

// Nombre d'utilisateurs sur le canal
size_t Channel::userCount() const { return users.size(); }
// Récupère la liste des utilisateurs
const std::vector<Client*>& Channel::getUsers() const { return users; }
// Récupère la liste des opérateurs
const std::vector<Client*>& Channel::getAdmins() const { return admins; }

// Active/désactive le mode "topic restriction" (+t)
void Channel::setTopicRestriction(bool b) 
{ 
    topicRestriction = b; 
}

// Vérifie si le canal est en mode "topic restriction" (+t)
bool Channel::isTopicRestricted() const {
    return topicRestriction;
}