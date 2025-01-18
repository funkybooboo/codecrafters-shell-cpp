#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <map>
#include <functional>

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
        return {}; // Return an empty vector if the start index is out of bounds
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
        std::cout << "exit: " + args[0] + ": invalid integer";
    }
}

[[noreturn]] int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Create a map to associate commands with their corresponding functions
    std::map<std::string, std::function<void(const std::vector<std::string>&)>> commandMap;

    // Add commands to the map
    commandMap["exit"] = exitCommand;

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

        // Check if the command exists in the map and invoke it
        if (auto it = commandMap.find(command); it != commandMap.end()) {
            // Command found, invoke the corresponding function
            it->second(args);
        } else {
            // Command not found, invoke the default handler
            commandNotFound(command);
        }
        std::cout << std::endl;
    }
}
