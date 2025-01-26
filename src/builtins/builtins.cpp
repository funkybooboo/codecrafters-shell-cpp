#include <utility>

#include "builtins.hpp"
#include "cd/cd.hpp"
#include "echo/echo.hpp"
#include "exit/exit.hpp"
#include "pwd/pwd.hpp"
#include "type/type.hpp"

namespace builtins
{
    Result::Result(const std::int8_t code, std::string  msg) : statusCode(code), message(std::move(msg)) {}

    std::vector<std::string> getKeys(const std::map<std::string, std::function<Result(const std::string&)>>& registry) {
        std::vector<std::string> keys;
        for (const auto& pair : registry) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    void loadRegistry()
    {
        registry["exit"] = exit;
        registry["echo"] = echo;
        registry["type"] = type;
        registry["pwd"] = pwd;
        registry["cd"] = cd;

        builtinNames = getKeys(registry);
    }
}
