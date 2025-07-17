#include <iostream>
int main()
{
    while ((pos = channels_str.find(',')) != std::string::npos) {
        channel_names.push_back(channels_str.substr(0, pos));
        channels_str.erase(0, pos + 1);
    }
}