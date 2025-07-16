#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <iostream>
#include "client.hpp"
class Client ;

class Channel
{
	private:
        std::string name;
        std::string topic;
    	std::vector<Client> users;
        std::vector<Client> admins;
        std::vector<Client> invites;

    public:
        Channel(const std::string &name);
        std::string getName();
        std::string getTopic ();
        std::addToAdmin(client *c);
        
};

#endif