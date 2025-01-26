#include <iostream>

#include "echo.hpp"

namespace builtins
{
    void echo(const std::string& argument)
    {
        bool isInDoubleQuote = false;
        bool isInSingleQuote = false;

        std::string output;

        int i = 0;
        while (i < argument.length())
        {
            const char c = argument[i];

            if (c == '\"')
            {
                isInDoubleQuote = !isInDoubleQuote;
                i++;
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

            if (isInSingleQuote)
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

        std::cout << output << std::endl;
    }
}
