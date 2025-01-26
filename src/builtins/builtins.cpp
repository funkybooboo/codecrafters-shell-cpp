#include "builtins.hpp"

#include "cd/cd.hpp"
#include "echo/echo.hpp"
#include "exit/exit.hpp"
#include "pwd/pwd.hpp"
#include "type/type.hpp"

namespace builtins
{
    void loadRegistry()
    {
        registry["exit"] = exit;
        registry["echo"] = echo;
        registry["type"] = type;
        registry["pwd"] = pwd;
        registry["cd"] = cd;
    }
}
