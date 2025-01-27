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

    void sort(std::vector<std::string>& words) {
        const int n = words.size();

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (words[j] > words[j + 1]) {
                    const std::string temp = words[j];
                    words[j] = words[j + 1];
                    words[j + 1] = temp;
                }
            }
        }
    }
}
