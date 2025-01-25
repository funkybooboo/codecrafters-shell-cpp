#pragma once

#include <string>
#include <map>
#include <functional>
#include <iostream>

namespace builtins {

    inline std::map<std::string, std::function<void(const std::string&)>> registry;

    void loadRegistry();

} // namespace builtins
