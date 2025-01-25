#include <iostream>
#include <cstdint>
#include <optional>
#include <filesystem>

#include "builtins.hpp"
#include "environment.hpp"

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

[[noreturn]] int main()
{
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

        std::string argument = input.substr(command.length());

        if (!argument.empty() && argument[0] == ' ')
        {
            argument = argument.substr(1);
        }

        if (const auto it = builtins::registry.find(command); it != builtins::registry.end())
        {
            it->second(argument);
            continue;
        }

        if (const std::optional<std::filesystem::path> result = environment::getCommandPath(command))
        {
            if (const std::filesystem::path& command_path = *result; environment::isExecutable(command_path))
            {
                std::system(input.c_str());
                continue;
            }
        }

        std::cout << input << ": command not found" << std::endl;
    }
}
