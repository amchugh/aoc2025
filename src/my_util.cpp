
#include "my_util.h"

#include <fstream>
#include <sstream>

// Read a file into a string for convenience.
std::string read_file_as_string(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

std::vector<std::string> to_lines(const std::string& text) {
    std::vector<std::string> result;
    size_t pos = 0;

    while (pos != text.size()) {
        const auto next = text.find('\n', pos);
        if (next == std::string::npos) {
            // add the end, if it's not a blank line
            break;
        }
        const std::string part(text.begin() + pos, text.begin() + next);
        result.push_back(part);
        pos = next + 1;
    }

    return result;
}
