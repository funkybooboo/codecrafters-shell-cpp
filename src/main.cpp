#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <map>
#include <functional>
#include <filesystem>

std::map<std::string, std::function<void(const std::vector<std::string>&)>> shellBuiltins;

std::vector<std::string> splitString(const std::string& str, const char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::string> sliceVector(const std::vector<std::string>& vec, const size_t startIndex) {
    if (startIndex >= vec.size()) {
        return {};
    }
    return {vec.begin() + static_cast<long>(startIndex), vec.end()};
}

std::vector<std::string> getPathsFromEnv() {
    std::vector<std::string> paths;

    if (const char* path = std::getenv("PATH")) {
        const std::string path_str(path);

        const char delimiter = (std::filesystem::is_directory("/")) ? ':' : ';';

        std::stringstream ss(path_str);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            paths.push_back(item);
        }
    }

    return paths;
}

bool isExecutable(const std::filesystem::path& path) {
    return exists(path) &&
           is_regular_file(path) &&
           (status(path).permissions() & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;
}

std::filesystem::path findCommandInPath(const std::string& command) {
    std::vector<std::string> paths = getPathsFromEnv();
    if (paths.empty()) {
        std::cout << "PATH environment variable not found or is empty." << std::endl;
    }

    for (const auto& dir : paths) {
        if (std::filesystem::path command_path = std::filesystem::path(dir) / command; isExecutable(command_path)) {
            return command_path;
        }
    }

    return "";
}

void exitCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        exit(0);
    }
    try {
        exit(std::stoi(args[0]));
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "exit: " + args[0] + ": invalid integer" << std::endl;
    }
}

void echoCommand(const std::vector<std::string>& args) {
    for (const std::string& arg : args) {
        std::cout << arg + " ";
    }
    std::cout << std::endl;
}

void typeCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cerr << "type: not enough arguments" << std::endl;
        return;
    }

    const std::string& command = args[0];

    if (const auto it = shellBuiltins.find(command); it != shellBuiltins.end()) {
        std::cout << it->first + " is a shell builtin" << std::endl;
        return;
    }

    const std::filesystem::path command_path = findCommandInPath(command);
    if (command_path.empty()) {
        std::cerr << command + ": not found" << std::endl;
        return;
    }
    std::cout << command + " is " + command_path.string() << std::endl;
}

void handleInput(const std::string& input) {
    const std::vector<std::string> parts = splitString(input, ' ');

    const std::string& command = parts[0];
    const std::vector<std::string> args = sliceVector(parts, 1);

    if (const auto it = shellBuiltins.find(command); it != shellBuiltins.end()) {
        it->second(args);
        return;
    }

    const std::filesystem::path command_path = findCommandInPath(command);
    if (command_path.empty()) {
        std::cerr << command + ": command not found" << std::endl;
        return;
    }

    std::string full_command = command_path.filename().string();
    for (const auto& arg : args) {
        full_command += " " + arg;
    }

    std::system(full_command.c_str());
}

[[noreturn]] int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    shellBuiltins["exit"] = exitCommand;
    shellBuiltins["echo"] = echoCommand;
    shellBuiltins["type"] = typeCommand;

    while (true) {
        std::cout << "$ ";

        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        handleInput(input);
    }
}
