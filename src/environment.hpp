#pragma once

#include <filesystem>
#include <iostream>
#include <vector>
#include <optional>

namespace environment
{
    std::optional<std::filesystem::path> getCommandPath(const std::string& command);

    bool isExecutable(const std::filesystem::path& path);
} // namespace environment
