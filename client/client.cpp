#include "../headers/client.hpp"

Client::Client()
{
}

Client::Client(int fd)
{
    this->fd = fd;
    has_nick = false;
    has_user = false;
    registered = false;
}

Client::~Client()
{
}

void    Client::set_nick(std::string nickname)
{
    this->nickname = nickname;
    has_nick = true;
}

void    Client::set_realname(std::string realname)
{
    this->realname = realname;
}

void    Client::set_user(std::string username)
{
    this->username = username;
    has_user = true;
}

void    Client::set_pass(std::string password)
{
    this->password = password;
}

void    Client::set_has_nick(bool b)
{
    has_nick = b;
}

void    Client::set_has_user(bool b)
{
    has_user = b;
}

void    Client::set_has_pass(bool b)
{
    has_pass = b;
}

void    Client::set_registered(bool b)
{
    registered = b;
}

std::string Client::get_nick()
{
    return (nickname);
}

std::string Client::get_user()
{
    return (username);
}

std::string Client::get_pass()
{
    return (password);
}

std::string Client::get_realname()
{
    return (realname);
}

bool    Client::get_has_nick()
{
    return (has_nick);
}

bool    Client::get_has_user()
{
    return (has_user);
}

bool    Client::get_has_pass()
{
    return (has_pass);
}

bool    Client::is_registered()
{
    return (registered);
}

int Client::get_fd()
{
    return (fd);
}
