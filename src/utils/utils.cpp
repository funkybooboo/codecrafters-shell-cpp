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

    int partition(std::vector<std::string>& words, const int low, const int high) {
        const std::string pivot = words[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (words[j] < pivot) {
                i++;
                std::swap(words[i], words[j]);
            }
        }

        std::swap(words[i + 1], words[high]);
        return i + 1;
    }

    void quicksort(std::vector<std::string>& words, const int low, const int high) {
        if (low < high) {
            const int pi = partition(words, low, high);
            quicksort(words, low, pi - 1);
            quicksort(words, pi + 1, high);
        }
    }

    void sort(std::vector<std::string>& words) {
        quicksort(words, 0, words.size() - 1);
    }
}
