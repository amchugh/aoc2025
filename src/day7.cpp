
#include <cassert>
#include <string>
#include <ostream>

#include "my_util.hpp"

int day7(const std::string& input, std::ostream& output) {
    const auto lines = to_lines(input);

    size_t width = lines[0].size();
    long beams[width];
    long next_beams[width];

    // Find the start
    for (size_t col = 0; col < width; col++) {
        beams[col] = lines[0][col] == 'S';
        // if (lines[0][col] == 'S') beams[col] = 1;
        // else beams[col] = 0;
    }

    size_t part1 = 0;
    for (size_t line_idx = 1; line_idx < lines.size(); line_idx++) {
        const auto& line = lines[line_idx];

        // Reset next
        memset(next_beams, 0, width * sizeof(long));

        // Scan across
        for (size_t idx = 0; idx < width; idx++) {
            // If there is a beam above, we either split or copy down
            // Edit:: actually far faster to run this no matter what
            // as if `!beams[idx]`, then adding it has no effect.
            // branchless ftw.
            // if (beams[idx]) {
                if (line[idx] == '^') {
                    next_beams[idx - 1] += beams[idx];
                    next_beams[idx + 1] += beams[idx];
                    part1 += 1;
                } else {
                    next_beams[idx] += beams[idx];
                }
            // }
            // Otherwise, we do nothing

            // (Make sure there are no adjacent beams)
            ASSERT(!(idx > 0 && line[idx] == '^' && line[idx-1] == '^'), "line " << line_idx << " @ idx=" << idx);
        }

        // Copy next
        memcpy(beams, next_beams, width * sizeof(long));
    }

    size_t part2 = 0;
    for (size_t i = 0; i < width; i++) {
        part2 += beams[i];
    }

    output << "Part 1: " << part1 << std::endl; // 1594
    output << "Part 2: " << part2 << std::endl; // 15650261281478

    return 0;
}
