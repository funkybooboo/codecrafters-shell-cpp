#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <map>
#include <functional>

// Create a map to associate commands with their corresponding functions
std::map<std::string, std::function<void(const std::vector<std::string>&)>> shellBuiltins;

std::vector<std::string> splitString(const std::string& str, const char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;

    // Read each token separated by the delimiter (in this case, a space)
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

void commandNotFound(const std::string& command) {
    std::cerr << command + ": command not found";
}

void exitCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        exit(0);
    }
    try {
        exit(std::stoi(args[0]));
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "exit: " + args[0] + ": invalid integer";
    }
}

void echoCommand(const std::vector<std::string>& args) {
    for (const std::string& arg : args) {
        std::cout << arg + " ";
    }
}

void typeCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cerr << "type: not enough arguments";
        return;
    }

    if (const auto it = shellBuiltins.find(args[0]); it != shellBuiltins.end()) {
        std::cout << it->first + " is a shell builtin";
    } else {
        std::cerr << args[0] + ": not found";
    }
}

[[noreturn]] int main() {
    // Flush after every std::cout / std:cerr
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

        std::vector<std::string> parts = splitString(input, ' ');

        const std::string& command = parts[0];
        const std::vector<std::string> args = sliceVector(parts, 1);

        if (const auto it = shellBuiltins.find(command); it != shellBuiltins.end()) {
            it->second(args);
        } else {
            commandNotFound(command);
        }
        std::cout << std::endl;
    }
}
