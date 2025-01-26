#include <filesystem>
#include <unistd.h>

#include "cd.hpp"

namespace builtins
{
    Result cd(const std::string& argument) {
        if (argument.empty()) {
            return {0, ""};
        }

        std::string newDirStr = argument;

        if (newDirStr == "~") {
            if (const char* homeDir = std::getenv("HOME"); homeDir != nullptr) {
                newDirStr = homeDir;
            }
            else {
                return {1, "cd: $HOME not set"};
            }
        }
        else if (newDirStr.starts_with("~/")) {
            if (const char* homeDir = std::getenv("HOME"); homeDir != nullptr) {
                newDirStr.replace(0, 1, homeDir);
            }
            else {
                return {1, "cd: $HOME not set"};
            }
        }

        std::filesystem::path newDir = newDirStr;

        if (!newDir.is_absolute()) {
            const std::filesystem::path currentDir = std::filesystem::current_path();
            newDir = currentDir / newDir;
        }

        if (!exists(newDir)) {
            return {1, "cd: " + newDir.string() + ": No such file or directory"};
        }

        if (chdir(newDir.c_str()) != 0) {
            return {1, "cd: " + newDir.string() + ": No such file or directory"};
        }

        return {0, ""};
    }
}
