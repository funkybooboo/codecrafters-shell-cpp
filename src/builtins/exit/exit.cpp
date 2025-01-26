#include "exit.hpp"

namespace builtins
{
    Result exit(const std::string& argument)
    {
        try {
            const std::int32_t statusCode = std::stoi(argument);
            std::exit(statusCode);
        } catch (const std::exception&) {
            std::exit(0);
        }
    }
}
