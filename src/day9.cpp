#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Vertical {
    int x;
    int ylow;
    int yhigh;
};

int day9(const std::string& input, std::ostream& output) {
    std::istringstream iss(input);

    long part1 = 0;

    std::vector<std::pair<int, int>> squares;

    long x, y;
    char _;

    while (iss >> x >> _ >> y) {
        for (const auto& sq : squares) {
            long dx = sq.first - x + 1;
            long dy = sq.second - y + 1;
            part1 = std::max(part1, dx * dy);
        }

        squares.emplace_back(x, y);
    }

    output << "Part 1: " << part1 << std::endl; // 4749929916
    assert(part1 == 4749929916);

    return 1;
}
