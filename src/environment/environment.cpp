#include <iostream>
#include <vector>

#include "environment.hpp"

namespace environment
{
    std::vector<std::string> getPaths()
    {
        std::vector<std::string> paths;

        if (const char* path = std::getenv("PATH"))
        {
            const std::string path_str(path);

            const char delimiter = (std::filesystem::is_directory("/")) ? ':' : ';';

            std::stringstream ss(path_str);
            std::string item;
            while (std::getline(ss, item, delimiter)) {
                paths.push_back(item);
            }
        }
        else
        {
            std::cout << "PATH environment variable not found or is empty.";
        }

        return paths;
    }

    std::vector<std::string> getCommands()
    {
        std::vector<std::string> commands;

        for (const auto& directory : getPaths())
        {
            if (std::filesystem::is_directory(directory))
            {
                for (const auto& entry : std::filesystem::directory_iterator(directory))
                {
                    if (is_regular_file(entry) && isExecutable(entry.path()))
                    {
                        commands.push_back(entry.path().filename().string());
                    }
                }
            }
        }
        return commands;
    }

    std::optional<std::filesystem::path> getCommandPath(const std::string& command)
    {

        for (std::vector<std::string> paths = getPaths(); const auto& directory : paths)
        {

            if (std::filesystem::path command_path = std::filesystem::path(directory) / command; exists(command_path) && is_regular_file(command_path))
            {
                return command_path;
            }
        }

        return {};
    }

    bool isExecutable(const std::filesystem::path& path)
    {
        try
        {
            const auto status = std::filesystem::status(path);
            return exists(status) && is_regular_file(status) &&
                   (status.permissions() & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
        }
        catch ([[maybe_unused]] const std::filesystem::filesystem_error& e)
        {
            return false;
        }
    }
}
