#include <iostream>

#include "builtins.hpp"
#include "environment.hpp"

namespace builtins
{
    void exit(const std::string& argument)
    {
        try {
            const std::int32_t statusCode = std::stoi(argument);
            std::exit(statusCode);
        } catch (const std::exception&) {
            std::exit(0);
        }
    }

    void echo(const std::string& argument)
    {
        std::cout << argument << std::endl;
    }

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

    void loadRegistry()
    {
        registry["exit"] = exit;
        registry["echo"] = echo;
        registry["type"] = type;
    }
} // namespace builtins
