#include "../headers/irc.hpp"

bool	ft_isdigit(char *num)
{
	size_t i = 0;
	while (i < strlen(num))
	{
		if (!isdigit(num[i]))
			return false;
		i++;
	}
	return true;
}

int	parse_input(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "number of args is incorrect!" << std::endl;
		return 0;
	}
	if (!ft_isdigit(av[1]))
	{
		std::cerr << "port is incorrect!" << std::endl;
		return 0;
	}
	if (atoi(av[1]) < 1024 || atoi(av[1]) > 65535)
	{
		std::cerr << "port not available!" << std::endl;
		return 0;
	}
	if (!av[2])
		return 0;
	return 1;
}