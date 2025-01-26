#include <iostream>
#include <filesystem>

#include "pwd.hpp"

namespace builtins
{
    void pwd([[maybe_unused]] const std::string& argument) {
        const std::filesystem::path current_path = std::filesystem::current_path();
        std::cout << current_path.string() << std::endl;
    }
}
