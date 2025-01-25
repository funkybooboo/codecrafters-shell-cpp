#pragma once

#include <map>
#include <functional>
#include <cstdint>
#include <optional>

namespace builtins
{
  inline std::map<std::string, std::function<std::optional<std::int32_t>(const std::string&)>> registry;

  void loadRegistry();
} // namespace builtins
