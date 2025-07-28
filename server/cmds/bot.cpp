#include "../../headers/server.hpp"

void Server::help(Client *c)
{
	send_msg(*c, "[============= IRC MANUAL =============]");
	send_msg(*c, "Available Commands:");
	send_msg(*c, "");
	send_msg(*c, "PASS <password>");
	send_msg(*c, "  - Authenticate yourself with the server using a password.");
	send_msg(*c, "  - Must be sent before NICK/USER if required by the server.");
	send_msg(*c, "");

	send_msg(*c, "USER <username> <hostname> <servername> <realname>");
	send_msg(*c, "  - Register a new user with the server.");
	send_msg(*c, "  - Example: USER john localhost server :John Doe");
	send_msg(*c, "");

	send_msg(*c, "NICK <nickname>");
	send_msg(*c, "  - Set or change your nickname.");
	send_msg(*c, "  - Example: NICK coolguy123");
	send_msg(*c, "");

	send_msg(*c, "JOIN <#channel>");
	send_msg(*c, "  - Join a channel or create one if it doesn't exist.");
	send_msg(*c, "  - Example: JOIN #general");
	send_msg(*c, "");

	send_msg(*c, "INVITE <nickname> <#channel>");
	send_msg(*c, "  - Invite a user to a channel you are in (must be an operator).");
	send_msg(*c, "  - Example: INVITE alice #general");
	send_msg(*c, "");

	send_msg(*c, "KICK <#channel> <user> [<reason>]");
	send_msg(*c, "  - Remove a user from a channel (must be an operator).");
	send_msg(*c, "  - Example: KICK #general bob :spamming");
	send_msg(*c, "");

	send_msg(*c, "[=======================================]");
}

std::string generateSequence(int length) {
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dist(0, alphabet.size() - 1);

    std::string result;
    for (int i = 0; i < length; ++i) {
        result += alphabet[dist(gen)];
    }

    return result;
}


void Server::bot (Client *c, std::vector <std::string> cmd){
	static std::string sequence ;
	if (!cmd.size())
		return ;
	if (cmd.size() > 2)
		send_msg(*c, ERR_INVALIDBOTPARAMS(cmd[0]));
	if (cmd.size() == 2)
	{
		if (sequence == cmd[1])
			send_msg(*c, "OHHH NIICE U WIN\r\n");
		else 
			send_msg(*c, "3NDK DAKIRA DYAL DBANA\r\n");
		return ;
	}
	sequence = generateSequence(6);
	send_msg(*c, "rember this sequence: " + sequence + "\r\n");
	sleep(3);
	for (int i = 0; i < 50 ; i++)
	{
		send_msg(*c, "\r\n");
	}
	send_msg(*c, "===========================================================\r\n");
}