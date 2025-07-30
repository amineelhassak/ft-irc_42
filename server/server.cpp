#include "../headers/server.hpp"

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
    this->name = "irc.com";
}

void Server::leave_channels(Client &c){
    for (int i = 0; i < allChannels.size(); i++)
    {
        allChannels[i].removeFromAdmin(&c);
        allChannels[i].removeFromChannel(&c);
    }
    std::cout << "hostname: " << c.get_hostname() << std::endl;
    std::string hot = "localhost";
    std::string msg = ":" + c.get_nick() + "!" + c.get_user() + "@" + hot + " QUIT :LEAVING\r\n";
    for (int i = 0; i < clients.size(); i++)
    {
        //:username!user@host QUIT :<quit message>
        if (clients[i].get_nick() != c.get_nick())
            send_msg(clients[i], msg);
    }
}

Server::~Server()
{
}

