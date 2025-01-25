#include <iostream>
#include <optional>
#include <filesystem>

#include "utils.hpp"
#include "builtins.hpp"
#include "environment.hpp"

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

        std::string command = utils::getToken(input);

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
