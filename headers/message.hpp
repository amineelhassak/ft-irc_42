#ifndef REPL_HPP
#define REPL_HPP

#define PREFIX ":irc.com "
#define POSTFIX "\r\n"

#define RPL_WELCOME_N "001"
#define RPL_YOURHOST_N "002"
#define RPL_CREATED_N "003"
#define RPL_MYINFO_N "004"
#define RPL_ISUPPORT_N "005"
#define RPL_MOTDSTART_N "375"
#define RPL_MOTD_N "372"
#define RPL_ENDOFMOTD_N "376"
#define ERR_UNKNOWNCOMMAND_N "421"
#define ERR_NONICKNAMEGIVEN_N "431"
#define ERR_NICKNAMEINUSE_N "433"
#define ERR_NEEDMOREPARAMS_N "461"
#define ERR_PASSWDMISMATCH_N "464"
#define ERR_NOTREGISTERED_N "451"
#define ERR_ERRONEUSNICKNAME_N "432"
 
#define RPL_NAMREPLY(sender, channel, users) PREFIX  "353 " + sender + " = " +  channel + " :" + users + POSTFIX
#define RPL_ENDOFNAMES(sender, channel) PREFIX  "366 " + sender + " " + channel + " :End of /NAMES list."  POSTFIX

#define RPL_JOINMSG(nickname, username, channelname)	(":" + nickname + "@" + username + " JOIN " + channelname + POSTFIX)

#define ERR_INVALIDBOTPARAMS(command) (PREFIX + std::string("B0T ") + std::string(command) + " :Invalid Number of Params" + POSTFIX)
#define ERR_NICKNAMEINUSE(target)						PREFIX "433 " + target + " :Nickname is already in use" POSTFIX
#define ERR_NEEDMOREPARAMS(command) (PREFIX + std::string("461 ") + std::string(command) + " :Not enough parameters" + POSTFIX)
#define ERR_UNKNOWNCOMMAND(command)						PREFIX "421 " + command + " :Unknown command" POSTFIX
#define ERR_CHANOPRIVSNEEDED(channel)					PREFIX "482 " + channel + " :You're not channel operator" POSTFIX
#define ERR_ERRONEUSNICKNAME(nick)						PREFIX "432 " + nick + " :Erroneus nickname" POSTFIX
#define ERR_NOTEXTTOSEND()								PREFIX "412 :No text to send" POSTFIX
#define ERR_NOSUCHCHANNEL(channel)						PREFIX "403 " + channel + " :No such channel" POSTFIX
#define ERR_NOSUCHNICK(nick)							PREFIX "401 " + nick + " :No such nick/channel" POSTFIX
#define ERR_NOTONCHANNEL(target, channel)	   			PREFIX "442 " + target + " " + channel + " :You're not on that channel" POSTFIX
#define ERR_USERNOTINCHANNEL(user, channel) 			PREFIX "441 " + user + " " + channel + " :They aren't on that channel" POSTFIX
#define ERR_USERONCHANNEL(user, channel) 				PREFIX "443 " + user + " " + channel + " :is already on channel" POSTFIX
#define ERR_NONICKNAMEGIVEN(target)           			PREFIX "431 " + target + " :No nickname given" POSTFIX
#define ERR_PASSWDMISMATCH(target)						PREFIX "464 " + target +":Password incorrect" POSTFIX
#define ERR_ALREADYREGISTRED(target)					PREFIX "462 " + target +":You may not reregister" POSTFIX
#define ERR_NOTREGISTERED								"Error: You are not registered."
#define ERR_CHANNELISFULL(client, channel)				PREFIX " 471 " + client + " " + channel + " :Cannot join channel (+l)" POSTFIX
#define ERR_INVITEONLYCHAN(client, channel)				PREFIX " 473 " + client + " " + channel + " :Cannot join channel (+i)" POSTFIX
#define ERR_BADCHANNELKEY(client, channel)				PREFIX " 475 " + client + " " + channel + " :Cannot join channel (+k)" POSTFIX
#define ERR_BANNEDFROMCHAN(client, channel)				PREFIX " 474 " + client + " " + channel + " :Cannot join channel (+b)" POSTFIX
#define ERR_BADCHANMASK(channel)						PREFIX " 476 " + channel + " :Bad Channel Mask" POSTFIX
#define ERR_NEEDMODEPARM(channelname, mode)				(std::string(": 696 ") + channelname + " * You must specify a parameter for the key mode. " + mode + POSTFIX)
#define ERR_INVALIDMODEPARM(channelname, mode)			(std::string(": 696 ") + channelname + " Invalid mode parameter. " + mode + POSTFIX)
#define ERR_UNKNOWNMODE(nickname, channelname, mode)	(std::string(": 472 ") + nickname + " " + channelname + " " + mode + " :is not a recognised channel mode" + POSTFIX)
#define ERR_INCORPASS(nickname)							(": 464 " + nickname + " :Password incorrect !" + POSTFIX )

#define ERR_NORECIPIENT(command)						PREFIX "411 " + command + " :No recipient given" POSTFIX
#define ERR_NOTEXTTOSEND()								PREFIX "412 :No text to send" POSTFIX
#define ERR_CANNOTSENDTOCHAN(channel)					PREFIX "44 " + channel + " :Cannot send to channel" POSTFIX
#define RPL_AWAY(nickname, message)						PREFIX "301 " + nickname + " : " + message + POSTFIX

#define RPL_WELCOME(sender, msg)               			PREFIX "001 " + sender + " : " + msg + POSTFIX

// #define RPL_NAMREPLY(sender, channel, users)    		PREFIX "353 " + sender + " = " + channel + " :" + users + POSTFIX
// #define RPL_ENDOFNAMES(sender, channel)        			PREFIX "366 " + sender + " " + channel + " :End of /NAMES list." POSTFIX



#define RPL_TOPIC(sender, channel, topic)				PREFIX " 332 " + sender + " " + channel + " :" + topic + POSTFIX
#define RPL_NOTOPIC(sender, channel)					PREFIX " 331 " + sender + " " + channel + " :No topic is set" + POSTFIX
#define RPL_PRIVMSG(nick, user, ipadd, target, msg)		":" + user + "!" + user + "@" + ipadd + " PRIVMSG " + target + " :" + msg + POSTFIX
#define RPL_NICK(sender, nick)							":" + sender + " NICK " + nick + POSTFIX
#define RPL_INVITING(nickname, targnick, targchan)  	": 341 " + nickname + " " + targnick + " " + targchan + POSTFIX
#define RPL_INVITE(sender, target, channel)				":" + sender + " INVITE " + target + " " + channel + POSTFIX
#define RPL_INVITING(nickname, targnick, targchan) 	 	": 341 " + nickname + " " + targnick + " " + targchan + POSTFIX
#define RPL_CHANGEMODE(hostname, channelname, mode)		(":" + hostname + " MODE " + channelname + " " + mode + POSTFIX)
#define RPL_UMODEIS(hostname, channelname, mode, user)	":" + hostname + " MODE " + channelname + " " + mode + " " + user + POSTFIX

#endif