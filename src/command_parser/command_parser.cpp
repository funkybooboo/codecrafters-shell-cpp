#include <cstdint>
#include <utility>
#include <functional>
#include <iostream>
#include <fstream>

#include "command_parser.hpp"
#include "../utils/utils.hpp"

namespace command_parser
{
    std::string getCommand(const std::string& input)
    {
        bool quotedCommand = false;
        bool isInDoubleQuote = false;
        bool isInSingleQuote = false;
        std::string command;
        for (const char c : input)
        {
            if (c == '\"' && !isInSingleQuote)
            {
                quotedCommand = true;
                isInDoubleQuote = !isInDoubleQuote;
                continue;
            }
            if (isInDoubleQuote)
            {
                command += c;
                continue;
            }
            if (c == '\'')
            {
                quotedCommand = true;
                isInSingleQuote = !isInSingleQuote;
                continue;
            }
            if (isInSingleQuote)
            {
                command += c;
                continue;
            }
            if (c == ' ')
            {
                break;
            }
            command += c;
        }
        if (quotedCommand)
        {
            if (!command.empty() && (command[0] == '\'' || command[0] == '\"')) {
                command = command.substr(1);
            }
            if (!command.empty() && (command[0] == '\'' || command[0] == '\"')) {
                command = command.substr(1);
            }
        }
        return command;
    }

    std::string getArgument(const std::string& input)
    {
        std::string argument;
        std::int32_t i = 0;
        while (i < input.length()) {
            const char c = input[i];
            if ((c == '>') || (c == '1' && i + 1 < input.length() - 1 && input[i + 1] == '>') || (c == '2'  && i + 1 < input.length() - 1 && input[i + 1] == '>'))
            {
                break;
            }
            argument += c;
            i++;
        }
        return argument;
    }

    std::string getStdoutRedirectFilePath(const std::string& input)
    {
        std::string redirectFilePath;
        std::int32_t i = 0;
        bool isMySpace = false;
        while (i < input.length())
        {
            const char c = input[i];
            if (c == '1' && i + 1 < input.length() - 1 && input[i + 1] == '>')
            {
                isMySpace = true;
                i += 2;
                continue;
            }

            if (c == '>')
            {
                isMySpace = true;
                i++;
                continue;
            }

            if (c == '2' && i + 1 < input.length() - 1 && input[i + 1] == '>')
            {
                isMySpace = false;
                i += 2;
                continue;
            }

            if (isMySpace)
            {
                redirectFilePath += c;
            }

            i++;
        }
        return redirectFilePath;
    }

    std::string getStderrRedirectFilePath(const std::string& input)
    {
        std::string redirectFilePath;
        std::int32_t i = 0;
        bool isMySpace = false;
        while (i < input.length())
        {
            const char c = input[i];
            if (c == '2' && i + 1 < input.length() - 1 && input[i + 1] == '>')
            {
                isMySpace = true;
                i += 2;
                continue;
            }

            if (c == '1' && i + 1 < input.length() - 1 && input[i + 1] == '>')
            {
                isMySpace = false;
                i += 2;
                continue;
            }

            if (c == '>')
            {
                isMySpace = false;
                i++;
                continue;
            }

            if (isMySpace)
            {
                redirectFilePath += c;
            }

            i++;
        }
        return redirectFilePath;
    }

    ParsedCommand::ParsedCommand(
            std::string cmd,
            std::string arg,
            std::ostream* out,
            std::ostream* err,
            std::ofstream* outF,
            std::ofstream* errF
        ) :
        command(std::move(cmd)),
        argument(std::move(arg)),
        outStream(out),
        errStream(err),
        outFile(outF),
        errFile(errF)
    {}

    ParsedCommand::~ParsedCommand()
    {
        if (outFile)
        {
            outFile->close();
            delete outFile;
        }
        if (errFile)
        {
            errFile->close();
            delete errFile;
        }
    }

    std::string trimAndExtract(std::string& input, const std::function<std::string(const std::string&)>& getResult) {
        input = utils::trim(input);
        std::string result = getResult(input);
        input = input.substr(result.length());
        result = utils::trim(result);
        input = utils::trim(input);
        return result;
    }

    std::tuple<std::ostream*, std::ofstream*> getRedirectionStreamAndFile(std::ostream& defaultStream, const std::string& redirectFilePath)
    {
        std::ostream* stream = &defaultStream;
        std::ofstream* file = nullptr;

        if (!redirectFilePath.empty())
        {
            file = new std::ofstream();
            file->open(redirectFilePath, std::ios::out | std::ios::app);
            if (!file->is_open())
            {
                std::cerr << "Failed to open file for redirection" << std::endl;
            }
            else
            {
                stream = file;
            }
        }

        return {stream, file};
    }

    ParsedCommand parseCommand(std::string input)
    {
        const std::string command = trimAndExtract(input, getCommand);
        const std::string argument = trimAndExtract(input, getArgument);
        const std::string stdoutRedirectFilePath = trimAndExtract(input, getStdoutRedirectFilePath);
        const std::string stderrRedirectFilePath = trimAndExtract(input, getStderrRedirectFilePath);

        auto [outStream, outFile] = getRedirectionStreamAndFile(std::cout, stdoutRedirectFilePath);
        auto [errStream, errFile] = getRedirectionStreamAndFile(std::cerr, stderrRedirectFilePath);

        return {command, argument, outStream, errStream, outFile, errFile};
    }
}
