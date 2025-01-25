#include "environment.hpp"

namespace environment
{
    std::vector<std::string> getPaths() {
        std::vector<std::string> paths;

        if (const char* path = std::getenv("PATH")) {
            const std::string path_str(path);

            const char delimiter = (std::filesystem::is_directory("/")) ? ':' : ';';

            std::stringstream ss(path_str);
            std::string item;
            while (std::getline(ss, item, delimiter)) {
                paths.push_back(item);
            }
        } else {
            std::cout << "PATH environment variable not found or is empty.";
        }

        return paths;
    }

    std::optional<std::filesystem::path> getCommandPath(const std::string& command) {
        std::vector<std::string> paths = getPaths();

        for (const auto& directory : paths) {
            std::filesystem::path command_path = std::filesystem::path(directory) / command;

            if (std::filesystem::exists(command_path) && std::filesystem::is_regular_file(command_path)) {
                return command_path;
            }
        }

        return {};
    }

    bool isExecutable(const std::filesystem::path& path) {
        try
        {
            auto status = std::filesystem::status(path);
            return std::filesystem::exists(status) &&
                   std::filesystem::is_regular_file(status) &&
                   (status.permissions() & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
        }
        catch (const std::filesystem::filesystem_error& e) {
            return false;
        }
    }
} // namespace environment
