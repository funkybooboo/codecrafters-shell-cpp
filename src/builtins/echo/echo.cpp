#include <iostream>

#include "echo.hpp"

namespace builtins
{
    void echo(const std::string& argument)
    {
        bool isInSingleQuote = false;
        std::string output;

        int i = 0;
        while (i < argument.length())
        {
            const char c = argument[i];
            if (c == '\'')
            {
                isInSingleQuote = !isInSingleQuote;
                i++;
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
