
#include <string>
#include <ostream>

int safe_get(const std::string& input, int width, int height, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return 0;
    return input[y * (width + 1) + x] == '@';
}

int my_reduce(const std::string* input, std::string* output, int width, int height) {
    // Clone
    *output = *input;

    size_t count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            size_t adjacent = 0;

            for (int dy = -1; dy <= 1; dy++)
                for (int dx = -1; dx <= 1; dx++)
                    if (dy != 0 || dx != 0)
                        adjacent += safe_get(*input, width, height, x + dx, y + dy);

            if (safe_get(*input, width, height, x, y) && adjacent < 4) {
                (*output)[y * (width + 1) + x] = '.';
                ++count;
            }
        }
    }
    return count;
}

int day4(const std::string& input, std::ostream& output) {
    // Figure out the sizes
    int width = input.find('\n');
    int height = (input.size() / width) - 1;

    std::string a, b;
    a = input;

    std::string *current, *next;
    current = &a;
    next = &b;

    size_t part1 = my_reduce(current, next, width, height);

    size_t part2 = 0;
    size_t reduced_last = part1;
    while (reduced_last > 0) {
        part2 += reduced_last;
        std::swap(current, next);
        reduced_last = my_reduce(current, next, width, height);
    }

    output << "Part 1: " << part1 << std::endl; // 1478
    output << "Part 2: " << part2 << std::endl; // 9120

    return 0;
}
