
#include <cassert>
#include <string>
#include <ostream>

#include "my_util.hpp"

struct range_t {
    long begin;
    long end;

    bool overlap(const range_t& other) const {
        return (begin <= other.begin && end >= other.begin)
            || (begin <= other.end   && end >= other.end  )
            // Not quite sure why we need to check both sides...
            // perhaps "this" can be contained by other?
            // I think that's the failure mode.
            || (other.begin <= begin && other.end >= begin)
            || (other.begin <= end   && other.end >= end  );
    }

    void combine(const range_t& other) {
        assert(overlap(other));
        begin = std::min(begin, other.begin);
        end = std::max(end, other.end);
    }
};

std::vector<range_t> parse_ranges(std::vector<std::string>::const_iterator& itr) {
    std::vector<range_t> ranges;

    while (true) {
        if (*itr == "") break;

        const auto& line = *itr;
        const auto middle = line.find('-');
        range_t range { .begin = atol(line.data()), .end = atol(line.data() + middle + 1) };
        assert(range.begin > 0 && range.end > 0);

        size_t candidate_reduce_idx = 0;
        for (; candidate_reduce_idx < ranges.size(); candidate_reduce_idx++) {
            auto& r = ranges[candidate_reduce_idx];
            if (r.overlap(range)) {
                r.combine(range);
                goto reduce_down;
            }
        }

        // We were unable to combine them.
        ranges.push_back(range);
        goto nextrange;

        reduce_down:
        // We now need to reduce all the way down, erasing any that we manage to combine.
        {
            auto& reducing = ranges[candidate_reduce_idx];
            for (size_t idx = candidate_reduce_idx + 1; idx < ranges.size(); idx++) {
                const auto& c = ranges[idx];
                if (reducing.overlap(c)) {
                    reducing.combine(c);

                    // Erase fast as order does not matter
                    ranges[idx] = ranges[ranges.size() - 1];
                    ranges.pop_back();
                }
            }
        }
        goto nextrange;

        nextrange:
        ++itr;
    }
    ++itr;

    return ranges;
}

int day5(const std::string& input, std::ostream& output) {
    // to_lines is 20% of execution time lmao. We could remove this, or transform it to an iterator.
    const auto lines = to_lines(input);

    // Read the ranges
    auto itr = lines.begin();
    // This function is another 20%
    std::vector<range_t> ranges = parse_ranges(itr);
    assert(itr != lines.end());

    // Now the product ids
    size_t part1 = 0;
    while (itr != lines.end()) {
        // This atol is slow too, but no real way around it.
        const auto id = atol(itr->data());

        for (const auto& range : ranges) {
            if (range.begin <= id && range.end >= id) {
                ++part1;
                goto nextid;
            }
        }

        nextid:
        ++itr;
    }

    size_t part2 = 0;
    for (const auto& range : ranges) {
        part2 += range.end - range.begin + 1;
    }

    output << "Part 1: " << part1 << std::endl; // 865
    output << "Part 2: " << part2 << std::endl; // 352556672963116

    return 0;
}
