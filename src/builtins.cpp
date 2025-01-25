#include <iostream>
#include <unistd.h>

#include "builtins.hpp"
#include "environment.hpp"
#include "utils.hpp"

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

    void pwd([[maybe_unused]] const std::string& argument) {
        const std::filesystem::path current_path = std::filesystem::current_path();
        std::cout << current_path.string() << std::endl;
    }

    void cd(const std::string& argument) {
        if (argument.empty()) {
            return;
        }

        std::string newDirStr = utils::getToken(argument);

        if (newDirStr == "~") {
            if (const char* homeDir = std::getenv("HOME"); homeDir != nullptr) {
                newDirStr = homeDir;
            }
            else {
                std::cerr << "cd: $HOME not set" << std::endl;
                return;
            }
        }
        else if (newDirStr.starts_with("~/")) {
            if (const char* homeDir = std::getenv("HOME"); homeDir != nullptr) {
                newDirStr.replace(0, 1, homeDir);
            }
            else {
                std::cerr << "cd: $HOME not set" << std::endl;
                return;
            }
        }

        std::filesystem::path newDir = newDirStr;

        if (!newDir.is_absolute()) {
            const std::filesystem::path currentDir = std::filesystem::current_path();
            newDir = currentDir / newDir;
        }

        if (!exists(newDir)) {
            std::cerr << "cd: " + newDir.string() + ": No such file or directory" << std::endl;
            return;
        }

        if (chdir(newDir.c_str()) != 0) {
            std::cerr << "cd: " + newDir.string() + ": No such file or directory" << std::endl;
        }
    }

    void loadRegistry()
    {
        registry["exit"] = exit;
        registry["echo"] = echo;
        registry["type"] = type;
        registry["pwd"] = pwd;
        registry["cd"] = cd;
    }
} // namespace builtins
