#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

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

    inline std::map<std::string, std::set<std::string>> completions;

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
                completions[key].insert(command);
            }
        }
    }

    std::vector<std::string> getMatches(const std::string& commandPart)
    {
        if (const auto it = completions.find(commandPart); it != completions.end())
        {
            std::vector commands(it->second.begin(), it->second.end());
            if (commands.empty())
            {
                return {};
            }
            return commands;
        }
        return {};
    }

    std::string getLongestCommonPrefix(const std::vector<std::string>& matches)
    {
        if (matches.empty())
            return "";

        std::string prefix = matches[0];
        for (const std::string& match : matches)
        {
            size_t i = 0;
            while (i < prefix.length() && i < match.length() && prefix[i] == match[i])
            {
                i++;
            }
            prefix = prefix.substr(0, i);
        }

        return prefix;
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

     std::string getInput()
    {
        std::cout << "$ ";

        std::string input;
        size_t cursorPos = 0;
        std::int8_t tabCount = 0;

        while (true)
        {
            if (const char c = getChar(); c == prompt_constants::NEWLINE)
            {
                std::cout << std::endl;
                break;
            }
            else if (c == prompt_constants::TAB)
            {
                tabCount++;
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

                std::vector<std::string> matches = getMatches(commandPart);

                utils::sort(matches);

                if (matches.empty())
                {
                    if (tabCount == 1)
                    {
                        std::cout << "\a";
                    }
                }
                else if (matches.size() == 1)
                {
                    if (tabCount == 1)
                    {
                        std::string match = matches[0];
                        match += " ";
                        input.replace(0, commandPart.length(), match);
                        cursorPos = match.length();
                        std::cout << "\r$ " << input;
                    }
                }
                else
                {
                    if (tabCount == 1)
                    {
                        if (std::string longestPrefix = getLongestCommonPrefix(matches); longestPrefix.length() > commandPart.length())
                        {
                            input.replace(0, commandPart.length(), longestPrefix);
                            cursorPos = longestPrefix.length();
                            std::cout << "\r$ " << input;
                        }
                        else
                        {
                            std::cout << "\a";
                        }
                    }
                    else if (tabCount == 2)
                    {
                        std::string matchList;
                        for (const std::string& match : matches)
                        {
                            matchList += match + "  ";
                        }
                        matchList.pop_back();
                        std::cout << std::endl << matchList << std::endl;
                        std::cout << "$ ";
                        std::cout << input;
                    }
                }
            }
            else
            {
                tabCount = 0;
                input.insert(cursorPos, 1, c);
                cursorPos++;
                std::cout << c;
            }
        }

        input = utils::trim(input);
        return input;
    }
}
