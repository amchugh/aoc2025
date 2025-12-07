
#include <cassert>
#include <string>
#include <ostream>

#include "my_util.h"

int day6(const std::string& input, std::ostream& output) {
    const auto lines = to_lines(input);

    std::vector<std::vector<int>> numbers;
    std::vector<char> operations;

    for (size_t i = 0; i < lines.size() - 1; i++) {
        const auto& line = lines[i];
        std::vector<int> line_numbers;

        size_t idx = 0;
        while (idx < line.size()) {
            // Move to the start of the next number
            while (line[idx] == ' ') idx++;
            // Grab the number
            const int number = atoi(line.data() + idx);
            output << number << std::endl;
            line_numbers.push_back(number);
            // Advance
            size_t next = line.find(' ', idx);
            if (next == std::string::npos) break;
            idx = next + 1;
        }

        numbers.push_back(line_numbers);
    }

    {
        const auto& ops_line = lines[lines.size() - 1];
        size_t idx = 0;
        while (idx < ops_line.size()) {
            // Move to the start of the next number
            while (ops_line[idx] == ' ') idx++;
            // Grab the number
            const char op = ops_line[idx];
            output << op << std::endl;
            operations.push_back(op);
            // Advance
            size_t next = ops_line.find(' ', idx);
            if (next == std::string::npos) break;
            idx = next + 1;
        }
    }

    // checks
    {
        for (const auto& nums: numbers) {
            assert(nums.size() == operations.size());
        }
    }

    size_t part1 = 0;
    for (size_t idx = 0; idx < operations.size(); idx++) {
        auto op = operations[idx];
        assert(op == '+' || op == '*');

        size_t acc;
        if (op == '+') acc = 0;
        else acc = 1;

        for (const auto& nums: numbers) {
            if (op == '+') acc += nums[idx];
            else acc *= nums[idx];
        }

        output << acc << std::endl;
        part1 += acc;
    }

    output << "----------------------------------------" << std::endl;

    // Part 2 is just so different... parsing in a completely different manner.
    // Will start over here :/

    size_t part2 = 0;
    size_t num_end = 0;
    size_t cols_processed = 0;
    while (num_end < lines[0].size()) {
        // Find the further space in any of the lines
        size_t start = num_end;
        for (size_t line_idx = 0; line_idx < lines.size() - 1; line_idx++) {
            // Advance until we find the first number
            size_t idx = start;
            while (lines[line_idx][idx] == ' ') idx++;
            size_t this_end = lines[line_idx].find(' ', idx);
            if (this_end == std::string::npos) this_end = lines[line_idx].size();
            // output <<  << std::endl;
            num_end = std::max(num_end, this_end);
        }
        output << start << "->" << num_end << std::endl;

        // Print for debugging
        char buffer[80];
        for (size_t line_idx = 0; line_idx < lines.size() - 1; line_idx++) {
            memcpy(buffer, lines[line_idx].data() + start, num_end - start);
            buffer[num_end - start] = 0;
            output << buffer << std::endl;
        }

        // Discover what operation we are performing
        char op = operations[cols_processed];
        output << op << std::endl;

        size_t acc;
        if (op == '+') acc = 0;
        else acc = 1;

        // Work right-to-left, top-to-bottom to parse the numbers
        // size_t col = num_end - 1;
        for (size_t col = num_end; col > start; col--) {
            size_t col_idx = col - 1;
            // output << col << ", " << start << std::endl;
            size_t parsed = 0;
            for (size_t row = 0; row < lines.size() - 1; row++) {
                // ASSERT(!(parsed > 0) || lines[row][col_idx] != ' ', parsed << ", '" << lines[row][col_idx] << "'");
                if (lines[row].size() > col_idx && lines[row][col_idx] != ' ') {
                    parsed = parsed * 10 + lines[row][col_idx] - '0';
                }
            }
            output << parsed << std::endl;

            if (op == '+') acc += parsed;
            else acc *= parsed;
        }
        part2 += acc;

        // Get rid of the spaces
        num_end++;

        cols_processed++;
    }

    output << "Part 1: " << part1 << std::endl; // 4580995422905
    output << "Part 2: " << part2 << std::endl; // 10875057285868

    return 1;
}
