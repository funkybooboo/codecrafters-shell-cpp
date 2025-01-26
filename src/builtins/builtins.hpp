#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <functional>

namespace builtins
{
    struct Result {
        std::int8_t statusCode;
        std::string message;

        Result(std::int8_t code, std::string   msg);
    };

    inline std::vector<std::string> builtinNames;

    inline std::map<std::string, std::function<Result(const std::string&)>> registry;

    void loadRegistry();
}
