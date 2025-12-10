
#include <algorithm>
#include <cassert>
#include <cmath>
#include <sstream>
#include <string>
#include <ostream>

struct Box {
    size_t x, y, z;
};

double distance(const Box& a, const Box& b) {
    size_t dx = a.x - b.x;
    size_t dy = a.y - b.y;
    size_t dz = a.z - b.z;
    size_t d2 = dx * dx + dy * dy + dz * dz;
    return sqrt(d2);
}

struct Record {
    double distance;
    size_t a_idx;
    size_t b_idx;
};

// Returns 1 if a new region was made
int connect(const Record& r, size_t *region_map, size_t *region_sizes, size_t &regions, size_t &region_num, const size_t& region_count) {
    // Check both
    size_t region_a = region_map[r.a_idx];
    size_t region_b = region_map[r.b_idx];

    // If they're already in the same region, skip.
    if (region_a == region_b && region_a != -1llu) return 0;

    // If they're both -1
    if (region_a == -1llu && region_b == -1llu) {
        region_map[r.a_idx] = region_num;
        region_map[r.b_idx] = region_num;
        region_sizes[region_num] = 2;
        region_num++;
        regions++;
        return 1;
    }
    else if (region_a != -1llu && region_b != -1llu) {
        // Here, we need to replace the larger region with the smaller
        size_t smaller = std::min(region_a, region_b);
        size_t larger = std::max(region_a, region_b);
        region_sizes[smaller] += region_sizes[larger];
        region_sizes[larger] = 0;
        for (size_t i = 0; i < region_count; i++) {
            if (region_map[i] == larger) {
                region_map[i] = smaller;
            }
        }
        regions--;
        return 0;
    }
    else {
        size_t existing = std::min(region_a, region_b);
        region_map[r.a_idx] = existing;
        region_map[r.b_idx] = existing;
        region_sizes[existing]++;
        return 0;
    }
}

int day8(const std::string& input, std::ostream& output) {
    // Number of connections to make for part1
    constexpr size_t N = 1000;

    // Parse the input
    std::vector<Box> boxes;
    {
        std::stringstream ss(input);
        size_t x, y, z;
        char _;

        while (ss >> x >> _ >> y >> _ >> z) {
            boxes.push_back(Box {.x = x, .y = y, .z = z});
        }
    }
    const size_t count = boxes.size();

    struct MinHeapComparator {
        bool operator()(const Record& a, const Record& b) const {
            return a.distance > b.distance; // For min-heap, treat 'a' as "less" if it's actually greater
        }
    };
    MinHeapComparator comp{};
    std::vector<Record> smallest_distances;
    smallest_distances.reserve(count * (count - 1) / 2);

    // Calculate all the distances
    for (size_t i = 0; i < boxes.size(); i++) {
        for (size_t j = i + 1; j < boxes.size(); j++) {
            const double dist = distance(boxes[i], boxes[j]);
            const Record record {.distance = dist, .a_idx = i, .b_idx = j};

            smallest_distances.push_back(record);
            std::push_heap(smallest_distances.begin(), smallest_distances.end(), comp);
        }
    }

    size_t regions = 0;
    size_t region_num = 0;
    size_t region_map[count];
    memset(region_map, 0xff, sizeof(region_map));
    assert(region_map[0] == -1llu);
    assert(region_map[count - 1] == -1llu);

    size_t region_sizes[count];
    memset(region_sizes, 0, sizeof(region_sizes));

    auto end = smallest_distances.end();
    for (size_t i = 0; i < N; i++) {
        const auto r = smallest_distances.front();
        // Reduce
        std::pop_heap(smallest_distances.begin(), end, comp);
        end--;

        auto _ = connect(
            r,
            region_map, region_sizes,
            regions, region_num, count
        );
    }

    // Find the 3 largest regions
    size_t largest1 = 0;
    size_t largest2 = 0;
    size_t largest3 = 0;

    for (size_t i = 0; i < region_num; i++) {
        const auto size = region_sizes[i];
        if (size > largest1) {
            largest3 = largest2;
            largest2 = largest1;
            largest1 = size;
        }
        else if (size > largest2) {
            largest3 = largest2;
            largest2 = size;
        }
        else if (size > largest3) {
            largest3 = size;
        }
    }
    size_t part1 = largest1 * largest2 * largest3;

    // Now keep going until they are one region
    size_t part2 = -1;
    while (end != smallest_distances.begin()) {
        const auto r = smallest_distances.front();
        // Reduce
        std::pop_heap(smallest_distances.begin(), end, comp);
        end--;

        auto added = connect(
            r,
            region_map, region_sizes,
            regions, region_num, count
        );

        if (!added && regions == 1) {
            // Need to see if all have been added
            // This should manifest as the region having a size equal to the number
            // of total boxes.
            const auto region = region_map[0];
            assert(region != -1llu);
            if (region != -1llu && region_sizes[region] == count) {
                // We found it!
                const auto &a = boxes[r.a_idx];
                const auto &b = boxes[r.b_idx];
                part2 = a.x * b.x;
                break;
            }
        }
    }

    output << "Part 1: " << part1 << std::endl; // 68112
    output << "Part 2: " << part2 << std::endl; // 44543856

    assert(part1 == 68112);
    assert(part2 == 44543856);

    return 0;
}
