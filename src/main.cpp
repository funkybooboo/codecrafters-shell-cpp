#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <map>
#include <functional>
#include <filesystem>
#include <unistd.h>

std::map<std::string, std::function<void(const std::vector<std::string>&)>> shellBuiltins;

std::vector<std::string> getParts(const std::string& input) {
    std::vector<std::string> tokens;
    std::string token;
    bool in_quotes = false;

    for (const char c : input) {
        if (c == '\'' || c == '"') {
            in_quotes = !in_quotes;
            token += c;
        }
        else if (c == ' ' && !in_quotes) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        else {
            token += c;
        }
    }

    if (!token.empty()) {
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

void pwdCommand(const std::vector<std::string>& args) {
    const std::filesystem::path current_path = std::filesystem::current_path();
    std::cout << current_path.string() << std::endl;
}

void cdCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        return;
    }

    std::string newDirStr = args[0];

    if (newDirStr == "~") {
        if (const char* homeDir = std::getenv("HOME"); homeDir != nullptr) {
            newDirStr = homeDir;
        }
        else {
            std::cerr << "cd: $HOME not set" << std::endl;
            return;
        }
    }
    else if (newDirStr.starts_with("~/")) {
        if (const char* homeDir = std::getenv("HOME"); homeDir != nullptr) {
            newDirStr.replace(0, 1, homeDir);
        }
        else {
            std::cerr << "cd: $HOME not set" << std::endl;
            return;
        }
    }

    std::filesystem::path newDir = newDirStr;

    if (!newDir.is_absolute()) {
        const std::filesystem::path currentDir = std::filesystem::current_path();
        newDir = currentDir / newDir;
    }

    if (!exists(newDir)) {
        std::cerr << "cd: " + newDir.string() + ": No such file or directory" << std::endl;
        return;
    }

    if (chdir(newDir.c_str()) != 0) {
        std::cerr << "cd: " + newDir.string() + ": No such file or directory" << std::endl;
    }
}

void handle(const std::string& input) {
    if (input.empty()) {
        return;
    }

    const std::vector<std::string> parts = getParts(input);

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
    shellBuiltins["pwd"] = pwdCommand;
    shellBuiltins["cd"] = cdCommand;

    while (true) {
        std::cout << "$ ";

        std::string input;
        std::getline(std::cin, input);

        handle(input);
    }
}
