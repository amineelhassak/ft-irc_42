#include "../headers/server.hpp"

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
    this->name = "irc.com";
}

void Server::leave_channels(Client &c){
    for (size_t i = 0; i < allChannels.size(); i++)
    {
        if (allChannels[i].hasClient(&c) == false)
            continue;
        allChannels[i].removeFromAdmin(&c);
        allChannels[i].removeFromChannel(&c);
        if (allChannels[i].getAdmins().size() == 0 && allChannels[i].getUsers().size())
        {
            Client *top = allChannels[i].getUsers()[0];
            if (!top)
                continue;
            allChannels[i].addToAdmin(top);
        }
    }
    std::string msg = ":" + c.get_nick() + "!" + c.get_user() + "@" + c.get_hostname() + " QUIT :LEAVING\r\n";
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].get_nick() != c.get_nick())
            send_msg(clients[i], msg);
    }
}

Server::~Server()
{
}

