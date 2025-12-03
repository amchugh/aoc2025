
#include <cassert>
#include <ostream>
#include <string>

#include "my_util.h"

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

    int total = 0;

    for (const auto& line : lines) {
        assert(line.size() > 2);

        // Find the largest digit other than the last
        size_t largest_idx = 0;
        for (size_t i = 1; i < line.size() - 1; i++) {
            if (line[i] > line[largest_idx]) {
                largest_idx = i;
            }
        }

        // Find the next largest digit after
        size_t second_largest = largest_idx + 1;
        for (size_t i = second_largest + 1; i < line.size(); i++) {
            if (line[i] > line[second_largest]) {
                second_largest = i;
            }
        }

        int val = (line[largest_idx] - '0') * 10 + line[second_largest] - '0';

        size_t first = find_largest(line, 0, line.size() - 1);
        size_t second = find_largest(line, first + 1, line.size());
        int other_val = (line[first] - '0') * 10 + line[second] - '0';

        ASSERT(val == other_val, "val: " << val << " other_val: " << other_val);

        total += val;
    }

    output << "Part 1: " << total << std::endl;

    return 1;
}
