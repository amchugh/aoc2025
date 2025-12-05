
#include <cassert>
#include <iterator>
#include <string>
#include <ostream>

#include "my_util.h"

struct range_t {
    long begin;
    long end;

    bool overlap(const range_t& other) {
        return (begin <= other.begin && end >= other.begin)
            || (begin <= other.end   && end >= other.end  );
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
        ranges.push_back(std::move(range));

        ++itr;
    }
    ++itr;

    return ranges;
}

int day5(const std::string& input, std::ostream& output) {
    const auto lines = to_lines(input);

    // Read the ranges
    auto itr = lines.begin();
    std::vector<range_t> ranges = parse_ranges(itr);
    assert(itr != lines.end());

    for (auto range : ranges) {
        output << range.begin << "-" << range.end << std::endl;
    }

    // Now the product ids
    size_t fresh_count = 0;
    while (itr != lines.end()) {
        const auto id = atol(itr->data());

        for (const auto& range : ranges) {
            if (range.begin <= id && range.end >= id) {
                ++fresh_count;
                goto nextid;
            }
        }

        nextid:
        ++itr;
    }

    output << "Part 1: " << fresh_count << std::endl; // 865

    // Now we need to reduce the ranges
    std::vector<range_t> reduced;
    reduced.reserve(ranges.size());
    reduced.push_back(ranges[0]);

    for (size_t idx = 1; idx < ranges.size(); idx++) {
        const auto& range = ranges[idx];
        size_t candidate_reduce_idx = 0;
        for (; candidate_reduce_idx < reduced.size(); candidate_reduce_idx++) {
            auto& r = reduced[candidate_reduce_idx];
            if (r.overlap(range)) {
                // output << "combining " << r.begin << "-" << r.end << " with " << range.begin << "-" << range.end;
                r.combine(range);
                // output << " into " << r.begin << "-" << r.end << std::endl;
                goto reduce_down;
            }
        }

        // We were unable to combine them.
        reduced.push_back(range);
        continue;

        reduce_down:
        // We now need to reduce all the way down, erasing any that we manage to combine.
        auto& reducing = reduced[candidate_reduce_idx];
        for (size_t idx = candidate_reduce_idx + 1; idx < reduced.size(); idx++) {
            const auto& c = reduced[idx];
            if (reducing.overlap(c)) {
                // output << "reducing down" << std::endl;
                reducing.combine(c);

                // Erase fast as order does not matter
                reduced[idx] = reduced[reduced.size() - 1];
                reduced.pop_back();
            }
        }
        continue;
    }

    for (auto range : reduced) {
        output << range.begin << "-" << range.end << std::endl;
    }

    size_t part2 = 0;
    for (auto range : reduced) {
        part2 += range.end - range.begin + 1;
    }

    output << "Part 2: " << part2 << std::endl;

    return 1;
}
