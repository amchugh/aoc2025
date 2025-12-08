
#include <cassert>
#include <cstdlib>
#include <iostream>

#include "my_util.hpp"

int day1(const std::string& input, std::ostream& output) {
    const auto lines = to_lines(input);

    size_t lands_on_zero = 0;
    int position = 50;

    size_t _extra_rotations = 0;
    size_t _rotations = 0;

    for (auto& line: lines) {
        const bool invert = line[0] == 'L';
        const int raw = atoi(&line[1]);

        assert(raw > 0);

        size_t extra_rotations = std::abs(raw) / 100;
        _extra_rotations += extra_rotations;

        const int val = invert ? -(raw % 100) : (raw % 100);
        const int new_pos = position + val;
        if (new_pos < 0) {
            if (position > 0) _rotations++;
            _rotations += std::abs(new_pos) / 100;
            position = 100 + (new_pos % 100);
            if (position == 100) position = 0;
        } else if (new_pos == 0) {
            _rotations++;
            position = 0;
        } else {
            _rotations += new_pos / 100;
            position = new_pos % 100;
        }

        if (position == 0) {
            lands_on_zero++;
        }

        ASSERT(position < 100 && position >= 0, "position: " << position);
    }

    output << "Part 1: " << lands_on_zero << std::endl;                      // 1182
    output << "Part 2: " << _extra_rotations + _rotations << std::endl;      // 6907

    return 0;
}
