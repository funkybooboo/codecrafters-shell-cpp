#include <vector>
#include <sstream>

#include "utils.hpp"

namespace utils
{
    std::string getToken(const std::string& input, const std::int32_t tokenIndex)
    {
        std::stringstream ss(input);
        std::string token;
        std::vector<std::string> tokens;

        while (ss >> token)
        {
            tokens.push_back(token);
        }

        if (tokenIndex >= 0 && tokenIndex < tokens.size())
        {
            return tokens[tokenIndex];
        }

        return "";
    }
} // namespace utils

