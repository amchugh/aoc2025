
#include <cassert>
#include <string>
#include <ostream>

#include "my_util.hpp"

int day6(const std::string& input, std::ostream& output) {
    const auto lines = to_lines(input);

    auto ops_itr = lines.back().begin();
    size_t part1 = 0;
    size_t part2 = 0;
    size_t num_end = 0;

    while (num_end < lines[0].size()) {
        // Find the further space in any of the lines
        size_t start = num_end;
        for (size_t line_idx = 0; line_idx < lines.size() - 1; line_idx++) {
            // Advance until we find the first number
            size_t idx = start;
            while (lines[line_idx][idx] == ' ') idx++;
            size_t this_end = lines[line_idx].find(' ', idx);
            if (this_end == std::string::npos) this_end = lines[line_idx].size();
            num_end = std::max(num_end, this_end);
        }
        // The numbers for this individual problem are from start -> num_end.

        // Discover what operation we are performing
        while (*(ops_itr) == ' ') { ++ops_itr; }
        char op = *(ops_itr++);

        size_t acc;

        // Part 1 is parsed simply, left-to-right
        if (op == '+') acc = 0;
        else acc = 1;
        for (size_t row = 0; row < lines.size() - 1; row++) {
            size_t parsed = atoi(lines[row].data() + start);
            if (op == '+') acc += parsed;
            else acc *= parsed;
        }
        part1 += acc;

        // Reset accumulator
        if (op == '+') acc = 0;
        else acc = 1;
        // Work right-to-left, top-to-bottom to parse the numbers for part 2
        for (size_t col = num_end; col > start; col--) {
            size_t col_idx = col - 1;
            size_t parsed = 0;
            for (size_t row = 0; row < lines.size() - 1; row++) {
                if (lines[row].size() > col_idx && lines[row][col_idx] != ' ') {
                    parsed = parsed * 10 + lines[row][col_idx] - '0';
                }
            }

            if (op == '+') acc += parsed;
            else acc *= parsed;
        }
        part2 += acc;

        // Get rid of the spaces
        num_end++;
    }

    // assert(part1 == 4580995422905);
    // assert(part2 == 10875057285868);

    output << "Part 1: " << part1 << std::endl; // 4580995422905
    output << "Part 2: " << part2 << std::endl; // 10875057285868

    return 0;
}
