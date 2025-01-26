#include <filesystem>

#include "pwd.hpp"

namespace builtins
{
    Result pwd([[maybe_unused]] const std::string& argument) {
        const std::filesystem::path current_path = std::filesystem::current_path();
        return {0, current_path.string()};
    }
}
