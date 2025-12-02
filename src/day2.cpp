
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
    // Let's brute force
    for (size_t n = 2; n <= x.size(); n++) {
        // Skip if not cleanly divisible.
        if (x.size() % n != 0) continue;
        size_t individual_size = x.size() / n;

        // Get all of the substrs with this size
        std::vector<std::string> substrs;
        for (size_t start_idx = 0; start_idx < x.size(); start_idx += individual_size) {
            substrs.push_back(x.substr(start_idx, individual_size));
        }
        assert(substrs.size() == n);

        // Now check if they all match
        for (size_t idx = 1; idx < n; idx++) {
            if (substrs[0] != substrs[idx]) goto nomatch;
        }
        return 1;

        // Continue otherwise
        nomatch:;
    }
    return 0;
}

int day2(const std::string& input) {
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

        for (size_t i = start; i <= end; i++) {
            const std::string x = std::to_string(i);
            part1 += is_symmetric(x) * i;
            part2 += only_repeats(x) * i;
        }

        idx = second_end + 1;
    }

    std::cout << "Part 1: " << part1 << std::endl; // 29940924880
    std::cout << "Part 2: " << part2 << std::endl; // 48631958998

    return 0;
}
