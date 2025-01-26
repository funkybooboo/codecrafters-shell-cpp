#include <iostream>

#include "type.hpp"

#include "../../environment/environment.hpp"
#include "../builtins.hpp"

namespace builtins
{
    void type(const std::string& argument)
    {
        if (const auto it = registry.find(argument); it != registry.end())
        {
            std::cout << it->first << " is a shell builtin" << std::endl;
            return;
        }

        if (const std::optional<std::filesystem::path> result = environment::getCommandPath(argument))
        {
            if (const std::filesystem::path& command_path = *result; environment::isExecutable(command_path))
            {
                std::cout << argument + " is " + command_path.string() << std::endl;
                return;
            }
        }

        std::cerr << argument + ": not found" << std::endl;
    }
}