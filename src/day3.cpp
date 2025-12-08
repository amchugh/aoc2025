
#include <cassert>
#include <ostream>
#include <string>

#include "my_util.hpp"

size_t find_largest(const std::string& str, size_t start_idx, size_t end_idx) {
    assert(start_idx < str.size());
    assert(end_idx <= str.size());
    assert(start_idx < end_idx);

    size_t largest_idx = start_idx;
    for (size_t idx = start_idx + 1; idx < end_idx; idx++) {
        if (str[idx] > str[largest_idx]) {
            largest_idx = idx;
        }
    }

    return largest_idx;
}

int day3(const std::string& input, std::ostream& output) {
    auto lines = to_lines(input);

    long part1 = 0;
    long part2 = 0;

    for (const auto& line : lines) {
        assert(line.size() > 2);

        {
            size_t first = find_largest(line, 0, line.size() - 1);
            size_t second = find_largest(line, first + 1, line.size());
            long val = (line[first] - '0') * 10 + line[second] - '0';
            part1 += val;
        }

        {
            size_t start_search_idx = 0;
            long val = 0;
            for (size_t i = 0; i < 12; i++) {
                size_t next_digit_idx = find_largest(line, start_search_idx, line.size() - 11 + i);

                val = val * 10 + line[next_digit_idx] - '0';

                start_search_idx = next_digit_idx + 1;
            }
            part2 += val;
        }
    }

    output << "Part 1: " << part1 << std::endl; // 17244
    output << "Part 2: " << part2 << std::endl; // 171435596092638

    return 0;
}
