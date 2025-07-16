#include "../headers/irc.hpp"
#include "../headers/client.hpp"
#include "../headers/server.hpp"

int main(int ac, char **av)
{
    if (!parse_input(ac, av))
        return 0;
    Server s(atoi(av[1]), av[2]);
    s.init_socket();
}
