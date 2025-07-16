#include "../headers/channel.hpp"

Channel::Channel(const std::string &name)
{
    this->name = name;
    this->topic = "";
}

std::string Channel::getName()
{
    return this->name;
}

std::string Channel::getTopic ()
{
    return this->topic;
}


void Channel::addToChannel(Client c)
{
    users.push_back(c);
}

void Channel::addToAdmin(Client c)
{
    admins.push_back(c);
}

int Channel::hasClient(Client &client) const {
    for (int i = 0;i < users.size();i++) {
        if (users[i] = &client) {
            return 1;
        }
    }
    return 0;
}


void Channel::addToChannelnon(Client &client) {
    if (this->hasClient(client)) {
        return;
    }
    this->_clients.push_back(&client);
    client.addChannel(this);
}