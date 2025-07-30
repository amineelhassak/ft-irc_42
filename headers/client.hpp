#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include "channel.hpp"
#include <algorithm>


//  #include "../headers/messages.hpp"

class Client
{
	private :
	int			fd;
	std::string	nickname;
	std::string	username;
	std::string	realname;
	std::string	hostname;
	std::string	servername;
	std::string	password;
	bool		has_nick;
	bool		has_user;
	bool		has_pass;
	bool		registered;
	// hostname
	// ipadress

	public :
	std::string buffer;
	Client();
	Client(int fd);
	~Client();
	void		set_nick(std::string nickname);
	void		set_user(std::string username);
	void		set_pass(std::string password);
	void		set_realname(std::string realname);
	void		set_hostname(std::string hostname);
	void		set_servername(std::string servername);
	void		set_has_nick(bool b);
	void		set_has_user(bool b);
	void		set_has_pass(bool b);
	void		set_registered(bool b);
	std::string	get_nick();
	std::string	get_user();
	std::string	get_pass();
	std::string	get_realname();
	std::string	get_hostname();
	std::string	get_servername();
	bool		get_has_nick();
	bool		get_has_user();
	bool		get_has_pass();
	bool		is_registered();
	int			get_fd();

};

#endif