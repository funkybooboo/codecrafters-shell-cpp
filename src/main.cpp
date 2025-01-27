#include <iostream>
#include <string>
#include <optional>
#include <filesystem>

#include "builtins/builtins.hpp"
#include "environment/environment.hpp"
#include "command_parser/command_parser.hpp"
#include "prompt_reader/prompt_reader.hpp"

[[noreturn]] int main()
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    builtins::loadRegistry();
    prompt_reader::loadCompletions();

    while (true)
    {
        std::string input = prompt_reader::getInput();

        if (input.empty())
        {
            continue;
        }

        auto parsedCommand = command_parser::parseCommand(input);

        if (const auto it = builtins::registry.find(parsedCommand.command); it != builtins::registry.end()) {
            const builtins::Result result = it->second(parsedCommand.argument);
            if (result.message.empty())
            {
                continue;
            }
            std::ostream& out = *((result.statusCode == 0) ? parsedCommand.outStream : parsedCommand.errStream);
            out << result.message << std::endl;
            continue;
        }

        if (const std::optional<std::filesystem::path> result = environment::getCommandPath(parsedCommand.command))
        {
            if (const std::filesystem::path& command_path = *result; environment::isExecutable(command_path))
            {
                std::system(input.c_str());
                continue;
            }
        }

        std::cout << input << ": command not found" << std::endl;
    }
}
