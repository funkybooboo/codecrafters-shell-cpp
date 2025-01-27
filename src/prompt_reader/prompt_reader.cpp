#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>

#include "../utils/utils.hpp"
#include "../builtins/builtins.hpp"
#include "../environment/environment.hpp"
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

    inline std::map<std::string, std::string> completions;

    void loadCompletions()
    {
        std::vector<std::string> commands = environment::getCommands();
        commands.insert(commands.end(), builtins::builtinNames.begin(), builtins::builtinNames.end());
        for (const std::string& command : commands)
        {
            std::string key;
            for (const char c : command)
            {
                key += c;
                completions[key] = command;
            }
        }
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

    std::string findCompletedCommand(const std::string& commandPart) {
        if (const auto it = completions.find(commandPart); it != completions.end())
        {
            return it->second;
        }
        std::cout << "\a";
        return commandPart;
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

        if (std::string match = findCompletedCommand(commandPart); match != commandPart)
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
