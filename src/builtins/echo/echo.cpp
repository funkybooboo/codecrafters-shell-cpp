#include "echo.hpp"

namespace builtins
{
    Result echo(const std::string& argument)
    {
        bool isInDoubleQuote = false;
        bool isInSingleQuote = false;

        std::string output;

        std::int32_t i = 0;
        while (i < argument.length())
        {
            const char c = argument[i];

            if (c == '\"' && !isInSingleQuote)
            {
                isInDoubleQuote = !isInDoubleQuote;
                i++;
                continue;
            }

            if (c == '\\' && isInDoubleQuote)
            {
                if (argument[i + 1] == '\\' || argument[i + 1] == '$' || argument[i + 1] == '\"')
                {
                    output += argument[i + 1];
                }
                i += 2;
                continue;
            }

            if (isInDoubleQuote)
            {
                output += c;
                i++;
                continue;
            }

            if (c == '\'')
            {
                isInSingleQuote = !isInSingleQuote;
                i++;
                continue;
            }

            if (isInSingleQuote)
            {
                output += c;
                i++;
                continue;
            }

            if (c == '\\')
            {
                output += argument[i + 1];
                i += 2;
                continue;
            }

            if (c != ' ')
            {
                output += c;
                i++;
                continue;
            }

            while (argument[i + 1] == ' ')
            {
                i++;
            }

            output += c;
            i++;
        }

        return {0, output};
    }
}
