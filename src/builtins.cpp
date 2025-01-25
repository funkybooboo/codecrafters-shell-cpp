#include <iostream>

#include "builtins.hpp"
#include "environment.hpp"

namespace builtins
{
    std::optional<std::int32_t> exit([[maybe_unused]] const std::string& argument)
    {
        return 0;
    }

    std::optional<std::int32_t> echo(const std::string& argument)
    {
        std::cout << argument << std::endl;
        return {};
    }

    std::optional<std::int32_t> type(const std::string& argument)
    {
        if (const auto it = registry.find(argument); it != registry.end())
        {
            std::cout << it->first << " is a shell builtin" << std::endl;
            return {};
        }

        if (const std::optional<std::filesystem::path> result = environment::getCommandPath(argument))
        {
            if (const std::filesystem::path& command_path = *result; environment::isExecutable(command_path))
            {
                std::cout << argument + " is " + command_path.string() << std::endl;
                return {};
            }
        }

        std::cerr << argument + ": not found" << std::endl;

        return {};
    }

    void loadRegistry()
    {
        registry["exit"] = exit;
        registry["echo"] = echo;
        registry["type"] = type;
    }
} // namespace builtins
