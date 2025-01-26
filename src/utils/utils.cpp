#include "utils.hpp"

namespace utils
{
    std::string trim(const std::string& str) {
        const size_t start = str.find_first_not_of(' ');
        if (start == std::string::npos) {
            return "";
        }

        const size_t end = str.find_last_not_of(' ');

        return str.substr(start, end - start + 1);
    }
}
