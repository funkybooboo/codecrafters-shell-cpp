#include <iostream>

#include "echo.hpp"

namespace builtins
{
    void echo(const std::string& argument)
    {
        bool isInSingleQuote = false;
        std::string output; // Accumulate characters to output

        for (const char c : argument)
        {
            if (c == '\'')
            {
                isInSingleQuote = !isInSingleQuote;
                continue;
            }

            if (c != ' ')
            {
                output += c;
                continue;
            }

            if (isInSingleQuote)
            {
                output += c;
            }
        }

        // Print the accumulated output
        std::cout << output << std::endl;
    }
}
