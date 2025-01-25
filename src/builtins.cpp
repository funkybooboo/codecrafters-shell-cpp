#include "builtins.hpp"

namespace builtins {

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
        }
        else
        {
            std::cout << argument << ": not found" << std::endl;
        }
    }

    void loadRegistry()
    {
        registry["exit"] = exit;
        registry["echo"] = echo;
        registry["type"] = type;
    }

} // namespace builtins
