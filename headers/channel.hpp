#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "client.hpp"
//  #include "../headers/messages.hpp"
class Client;

class Channel {
private:
    std::string name;
    std::string topic;
    std::vector<Client*> users;
    std::vector<Client*> admins;
    std::set<Client*> invites;
    std::set<std::string> banMasks;
    std::string key; // Channel password
    int userLimit;
    bool inviteOnly;
    bool topicRestriction; // Channel mode +t (topic restriction)
    // Channel modes: i (invite-only), k (key), l (limit), b (ban), o (op), t (topic restriction)
public:
    // void checkAdmin(Channel &c);
    void removeFromAdmin(Client* client);
    Channel(const std::string &name);
    std::string getName() const;
    std::string getTopic() const;
    void setTopic(const std::string& t);
    void addToAdmin(Client* c);
    void removeFromChannel(Client* c);
    bool hasClient(Client* c) const;
    void invite(Client* c);
    bool isInvited(Client* c) const;
    void setInviteOnly(bool b);
    bool isInviteOnly() const;
    void setTopicRestriction(bool b);
    bool isTopicRestricted() const;
    void setKey(const std::string& k);
    std::string getKey() const;
    void setUserLimit(int limit);
    int getUserLimit() const;
    size_t userCount() const;
    const std::vector<Client*>& getUsers() const;
    const std::vector<Client*>& getAdmins() const;
    void addToChannel(Client* c);
};

#endif