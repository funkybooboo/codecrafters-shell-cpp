#pragma once

#include <string>

namespace command_parser
{
    struct ParsedCommand {
        std::string command;
        std::string argument;
        std::ostream* outStream{};
        std::ostream* errStream{};
        std::ofstream* outFile{};
        std::ofstream* errFile{};

        ParsedCommand(
            std::string cmd,
            std::string arg,
            std::ostream* out,
            std::ostream* err,
            std::ofstream* outF,
            std::ofstream* errF
        );

        ~ParsedCommand();
    };

    ParsedCommand parseCommand(std::string input);
}
