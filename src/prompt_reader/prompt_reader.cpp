#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>

#include "../utils/utils.hpp"
#include "../builtins/builtins.hpp"
#include "prompt_reader.hpp"

namespace prompt_reader
{
    namespace prompt_constants
    {
        constexpr char NEWLINE = '\n';
        constexpr char TAB = '\t';
        constexpr char BACKSPACE = '\b';
        constexpr char DELETE = 127;
    }

    char getChar()
    {
        termios oldt{}, newt{};
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        const int c = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        if (c == EOF)
        {
            return EOF;
        }

        return static_cast<char>(c);
    }

    std::string findClosestMatch(const std::string& input, const std::vector<std::string>& builtinNames) {
        std::string closestMatch;

        for (const std::string& builtinName : builtinNames) {
            if (builtinName.find(input) == 0) {
                return builtinName;
            }
        }

        return input;
    }

    void handleTabCompletion(std::string& input, size_t& cursorPos)
    {
        const size_t commandEndPos = input.find_first_of(' ');

        std::string commandPart;
        if (commandEndPos != std::string::npos)
        {
            commandPart = input.substr(0, commandEndPos);
        }
        else
        {
            commandPart = input;
        }

        if (std::string match = findClosestMatch(commandPart, builtins::builtinNames); match != commandPart)
        {
            match += " ";
            input.replace(0, commandPart.length(), match);
            cursorPos = match.length();
            std::cout << "\r$ " << input;
        }
    }

    std::string getInput()
    {
        std::cout << "$ ";

        std::string input;
        size_t cursorPos = 0;

        while (true)
        {
            if (const char c = getChar(); c == prompt_constants::NEWLINE)
            {
                std::cout << std::endl;
                break;
            }
            else if (c == prompt_constants::TAB)
            {
                handleTabCompletion(input, cursorPos);
            }
            else
            {
                input.insert(cursorPos, 1, c);
                cursorPos++;
                std::cout << c;
            }
        }

        input = utils::trim(input);
        return input;
    }
}
