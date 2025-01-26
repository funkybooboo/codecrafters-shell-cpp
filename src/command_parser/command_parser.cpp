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

    bool isEqual(const std::string &str, const std::int32_t i, const char what)
    {
        return i < str.length() - 1 && str[i] == what;
    }

    std::string getArgument(const std::string& input)
    {
        std::string argument;
        std::int32_t i = 0;
        while (i < input.length()) {
            if (
                isEqual(input, i, '>') ||
                (isEqual(input, i, '>') && isEqual(input, i + 1, '>')) ||
                (isEqual(input, i, '1') && isEqual(input, i + 1, '>')) ||
                (isEqual(input, i, '1') && isEqual(input, i + 1, '>') && isEqual(input, i + 2, '>')) ||
                (isEqual(input, i, '2') && isEqual(input, i + 1, '>')) ||
                (isEqual(input, i, '2') && isEqual(input, i + 1, '>') && isEqual(input, i + 2, '>'))
                )
            {
                break;
            }
            argument += input[i];
            i++;
        }
        return argument;
    }

    std::string getStdoutRedirect(const std::string& input)
    {
        std::string redirectFilePath;
        bool isMySpace = false;

        std::int32_t i = 0;
        while (i < input.length())
        {
            if (isEqual(input, i, '2') && isEqual(input, i + 1, '>') && isEqual(input, i + 2, '>'))
            {
                isMySpace = false;
                i += 3;
                continue;
            }

            if (isEqual(input, i, '2') && isEqual(input, i + 1, '>'))
            {
                isMySpace = false;
                i += 2;
                continue;
            }

            if (isEqual(input, i, '1') && isEqual(input, i + 1, '>') && isEqual(input, i + 2, '>'))
            {
                isMySpace = true;
                redirectFilePath += input[i];
                redirectFilePath += input[i + 1];
                redirectFilePath += input[i + 2];
                i += 3;
                continue;
            }

            if (isEqual(input, i, '1') && isEqual(input, i + 1, '>'))
            {
                isMySpace = true;
                redirectFilePath += input[i];
                redirectFilePath += input[i + 1];
                i += 2;
                continue;
            }

            if (isEqual(input, i, '>') && isEqual(input, i + 1, '>'))
            {
                isMySpace = true;
                redirectFilePath += input[i];
                redirectFilePath += input[i + 1];
                i += 2;
                continue;
            }

            if (isEqual(input, i, '>'))
            {
                isMySpace = true;
                redirectFilePath += input[i];
                i++;
                continue;
            }

            if (isMySpace)
            {
                redirectFilePath += input[i];
            }

            i++;
        }
        return redirectFilePath;
    }

    std::string getStderrRedirect(const std::string& input)
    {
        std::string redirectFilePath;
        bool isMySpace = false;

        std::int32_t i = 0;
        while (i < input.length())
        {
            if (isEqual(input, i, '1') && isEqual(input, i + 1, '>') && isEqual(input, i + 2, '>'))
            {
                isMySpace = false;
                i += 3;
                continue;
            }

            if (isEqual(input, i, '1') && isEqual(input, i + 1, '>'))
            {
                isMySpace = false;
                i += 2;
                continue;
            }

            if (isEqual(input, i, '>') && isEqual(input, i + 1, '>'))
            {
                isMySpace = false;
                i += 2;
                continue;
            }

            if (isEqual(input, i, '>'))
            {
                isMySpace = false;
                i++;
                continue;
            }

            if (isEqual(input, i, '2') && isEqual(input, i + 1, '>') && isEqual(input, i + 2, '>'))
            {
                isMySpace = true;
                redirectFilePath += input[i];
                redirectFilePath += input[i + 1];
                redirectFilePath += input[i + 2];
                i += 3;
                continue;
            }

            if (isEqual(input, i, '2') && isEqual(input, i + 1, '>'))
            {
                isMySpace = true;
                redirectFilePath += input[i];
                redirectFilePath += input[i + 1];
                i += 2;
                continue;
            }

            if (isMySpace)
            {
                redirectFilePath += input[i];
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
        if (input.empty())
        {
            return "";
        }
        std::string result = getResult(input);
        input = input.substr(result.length());
        result = utils::trim(result);
        input = utils::trim(input);
        return result;
    }

    std::ofstream* attemptFileOpen(const std::string& filePath, const std::ios_base::openmode openMode)
    {
        if (const std::ifstream checkFile(filePath); !checkFile.good() || (openMode & std::ios::trunc))
        {
            const auto file = new std::ofstream();
            file->open(filePath, openMode);

            if (file->is_open())
            {
                return file;
            }
            delete file;
            return nullptr;
        }
        return nullptr;
    }

    std::tuple<std::ostream*, std::ofstream*> getRedirectionStreamAndFile(std::ostream& defaultStream, const std::ios_base::openmode openMode, const std::string& redirectFilePath)
    {
        std::ostream* stream = &defaultStream;
        std::ofstream* file = nullptr;

        if (!redirectFilePath.empty())
        {
            file = attemptFileOpen(redirectFilePath, openMode);
            stream = file;
        }

        return {stream, file};
    }

    std::tuple<std::ios_base::openmode, std::string> getStdoutOpenMode(const std::string &redirect)
    {
        std::ios_base::openmode openMode = std::ios::out | std::ios::app;
        std::string redirectPath;

        std::int32_t i = 0;
        while (i < redirect.length())
        {
            if (isEqual(redirect, i, '>') && isEqual(redirect, i + 1, '>'))
            {
                openMode = std::ios::app;
                i += 2;
                continue;
            }

            if (isEqual(redirect, i, '>'))
            {
                openMode = std::ios::out;
                i += 1;
                continue;
            }

            if (isEqual(redirect, i, '1') && isEqual(redirect, i + 1, '>') && isEqual(redirect, i + 2, '>'))
            {
                openMode = std::ios::app;
                i += 3;
                continue;
            }

            if (isEqual(redirect, i, '1') && isEqual(redirect, i + 1, '>'))
            {
                openMode = std::ios::out;
                i += 2;
                continue;
            }

            redirectPath += redirect[i];;
            i++;
        }

        return {openMode, redirectPath};
    }

    std::tuple<std::ios_base::openmode, std::string> getStderrOpenMode(const std::string &redirect)
    {
        std::ios_base::openmode openMode = std::ios::out | std::ios::app;
        std::string redirectPath;

        std::int32_t i = 0;
        while (i < redirect.length())
        {
            if (isEqual(redirect, i, '2') && isEqual(redirect, i + 1, '>') && isEqual(redirect, i + 2, '>'))
            {
                openMode = std::ios::app;
                i += 3;
                continue;
            }

            if (isEqual(redirect, i, '2') && isEqual(redirect, i + 1, '>'))
            {
                openMode = std::ios::out;
                i += 2;
                continue;
            }

            redirectPath += redirect[i];
            i++;
        }

        return {openMode, redirectPath};
    }

    ParsedCommand parseCommand(std::string input)
    {
        const std::string command = trimAndExtract(input, getCommand);
        const std::string argument = trimAndExtract(input, getArgument);
        const std::string stdoutRedirect = trimAndExtract(input, getStdoutRedirect);
        const std::string stderrRedirect = trimAndExtract(input, getStderrRedirect);

        auto [stdoutOpenMode, stdoutRedirectPath] = getStdoutOpenMode(stdoutRedirect);
        auto [stderrOpenMode, stderrRedirectPath] = getStderrOpenMode(stderrRedirect);

        auto [outStream, outFile] = getRedirectionStreamAndFile(std::cout, stdoutOpenMode, stdoutRedirectPath);
        auto [errStream, errFile] = getRedirectionStreamAndFile(std::cerr, stderrOpenMode, stderrRedirectPath);

        return {command, argument, outStream, errStream, outFile, errFile};
    }
}
