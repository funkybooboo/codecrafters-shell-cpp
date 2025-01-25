#include "builtins.hpp"

namespace builtins
{
    void exit(const std::string& argument)
    {
        std::exit(0);
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

        std::optional<std::filesystem::path> result = environment::getCommandPath(argument);

        if (result)
        {
            std::filesystem::path command_path = *result;
            if (environment::isExecutable(command_path))
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
