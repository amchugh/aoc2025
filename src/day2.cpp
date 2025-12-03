
#include "my_util.h"
#include <cassert>
#include <cstdlib>
#include <string>
#include <iostream>

int is_symmetric(const std::string& x) {
    // Not symmetric if odd-lengthed
    if (x.size() % 2 == 1) return 0;

    // Two iterators and we check that they match.
    auto itr1 = x.begin();
    auto itr2 = x.begin() + x.size() / 2;

    while (itr2 != x.end()) {
        if (*itr1 != *itr2) return 0;
        ++itr1;
        ++itr2;
    }

    return 1;
}

int only_repeats(const std::string& x) {
    std::string::const_iterator itrs[x.size()];

    // Let's brute force
    for (size_t n = 2; n <= x.size(); n++) {
        // Skip if not cleanly divisible.
        if (x.size() % n != 0) continue;
        size_t individual_size = x.size() / n;

        // Get iterators to all distinct parts
        for (size_t idx = 0; idx < n; idx++) {
            itrs[idx] = x.begin() + (idx * individual_size);
        }

        // Now, iterate all of them until the last hits the end or they are different
        while (itrs[n - 1] != x.end()) {
            for (size_t idx = 1; idx < n; idx++) {
                if (*itrs[0] != *itrs[idx]) goto nomatch;
                ++itrs[idx];
            }
            ++itrs[0];
        }
        return 1;

        // Continue otherwise
        nomatch:;
    }
    return 0;
}

int day2(const std::string& input, std::ostream& output) {
    size_t part1 = 0;
    size_t part2 = 0;

    size_t idx = 0;
    while (idx < input.size()) {
        size_t first_end = input.find('-', idx);
        size_t second_end = input.find(',', idx);
        if (second_end == std::string::npos) {
            second_end = input.find('\n', idx);
        }

        std::string first = input.substr(idx, first_end - idx);
        std::string second = input.substr(first_end + 1, second_end - first_end - 1);

        size_t start = std::stol(first);
        size_t end = std::stol(second);

        // We can be tricky
        size_t n = end - start;
        std::string current = first;
        for (size_t i = 0; i < n; i++) {
            part1 += is_symmetric(current) * (start + i);
            part2 += only_repeats(current) * (start + i);

            // Now increment the string directly.
            int idx = current.size() - 1;
            while (idx >= 0) {
                current[idx] += 1;
                if (current[idx] <= '9') goto next_number;

                assert(current[idx] == '9' + 1);

                current[idx] = '0';
                idx--;
            }
            assert(idx == -1);
            // Need to alloc more :(
            current[0] = '1';
            current.push_back('0');

            next_number:;
        }

        idx = second_end + 1;
    }

    output << "Part 1: " << part1 << std::endl; // 29940924880
    output << "Part 2: " << part2 << std::endl; // 48631958998

    return 0;
}
