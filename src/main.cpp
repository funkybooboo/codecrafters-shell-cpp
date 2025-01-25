#include "builtins.hpp"

std::string getCommand(const std::string& input)
{
    std::string command;
    for (const char c : input)
    {
        if (c == ' ')
        {
            break;
        }
        command += c;
    }
    return command;
}

[[noreturn]]int main() {
    builtins::loadRegistry();

    while (true)
    {
        std::cout << "$ ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty())
        {
            continue;
        }

        std::string command = getCommand(input);

        std::string argument = input.substr(command.length() + 1);

        if (const auto it = builtins::registry.find(command); it != builtins::registry.end())
        {
            it->second(argument);
        }
        else
        {
            std::cout << command << ": command not found" << std::endl;
        }
    }
}
