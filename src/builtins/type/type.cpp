#include "type.hpp"

#include "../../environment/environment.hpp"

namespace builtins
{
    Result type(const std::string& argument)
    {
        if (const auto it = registry.find(argument); it != registry.end())
        {
            return {0, it->first + " is a shell builtin"};
        }

        if (const std::optional<std::filesystem::path> result = environment::getCommandPath(argument))
        {
            if (const std::filesystem::path& command_path = *result; environment::isExecutable(command_path))
            {
                return {0, argument + " is " + command_path.string()};
            }
        }

        return {1, argument + ": not found"};
    }
}