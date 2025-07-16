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

void Client::addToChannel(Client *c)
{
	this->users.push_back(c);
}

void Client::addToAdmin(client *c)
{
    this->admins.push_back(c);
}