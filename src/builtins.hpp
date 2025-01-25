#pragma once

#include <map>
#include <functional>

namespace builtins
{
  inline std::map<std::string, std::function<void(const std::string&)>> registry;

  void loadRegistry();
} // namespace builtins
