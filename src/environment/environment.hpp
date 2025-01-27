#pragma once

#include <filesystem>
#include <optional>

namespace environment
{
    std::vector<std::string> getCommands();

    std::optional<std::filesystem::path> getCommandPath(const std::string& command);

    bool isExecutable(const std::filesystem::path& path);
}
