#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "cd.hpp"

namespace builtins
{
    void cd(const std::string& argument) {
        if (argument.empty()) {
            return;
        }

        std::string newDirStr = argument;

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
}
