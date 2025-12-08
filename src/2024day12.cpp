
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <string>
#include <sstream>

#include "my_util.hpp"

int in_bounds(const std::vector<std::string>& grid, int x, int y) {
    return x >= 0 && static_cast<size_t>(x) < grid[0].size()
        && y >= 0 && static_cast<size_t>(y) < grid.size();
}

typedef char direction_t;
char DIRECTIONS[] = ">V<^";
// 0 = right, 1 = down, 2 = left, 3 = up

struct Location {
    size_t x;
    size_t y;
    direction_t direction = 0;

    bool operator==(const Location& other) const {
        return x == other.x && y == other.y && direction == other.direction;
    }
};

void walk(
    const std::vector<std::string>& grid,
    const std::vector<Location>& in_region,
    int x, int y,
    size_t& perimeter,
    size_t& turns,
    std::vector<Location>& visited
) {
    // We will by default be moving right. We always move clockwise.
    direction_t direction = 0;
    int cx = x;
    int cy = y;

    do {
        Location current(cx, cy, direction);
        ASSERT(std::find(visited.begin(), visited.end(), current) == visited.end(), "Already visited: (" << cx << ", " << cy << ", " << DIRECTIONS[static_cast<size_t>(direction)] << ")");
        ASSERT(std::find(in_region.begin(), in_region.end(), Location(cx, cy)) != in_region.end(), "Current position: (" << cx << ", " << cy << ")");

        perimeter++;
        visited.push_back(current);

        int inside_ahead_x = cx;
        int inside_ahead_y = cy;
        if (direction == 0) { inside_ahead_x++; }
        else if (direction == 1) { inside_ahead_y++; }
        else if (direction == 2) { inside_ahead_x--; }
        else { inside_ahead_y--; }
        bool inside_ahead_in_region = std::find(in_region.begin(), in_region.end(), Location(inside_ahead_x, inside_ahead_y)) != in_region.end();

        int outside_ahead_x = cx;
        int outside_ahead_y = cy;
        if (direction == 0) { outside_ahead_x++; outside_ahead_y--; }
        else if (direction == 1) { outside_ahead_y++; outside_ahead_x++; }
        else if (direction == 2) { outside_ahead_x--; outside_ahead_y++; }
        else { outside_ahead_y--; outside_ahead_x--; }
        bool outside_ahead_in_region = std::find(in_region.begin(), in_region.end(), Location(outside_ahead_x, outside_ahead_y)) != in_region.end();

        // Is there a wall in our way?
        if (outside_ahead_in_region && in_bounds(grid, outside_ahead_x, outside_ahead_y)) {
            turns++;
            // Update the location. We go forward and left once
            if (direction == 0) { cx++; cy--; }
            else if (direction == 1) { cy++; cx++; }
            else if (direction == 2) { cx--; cy++; }
            else { cy--; cx--; }
            // Rotate outside
            direction = (direction + 3) % 4;
        }
        // Otherwise, do we need to round an exterior corner?
        else if (!inside_ahead_in_region) {
            turns++;
            // Rotate inside
            direction = (direction + 1) % 4;
            // Location doesn't need to be updated because we are spinning.
        }
        // March forward
        else {
            if (direction == 0) cx++;
            else if (direction == 1) cy++;
            else if (direction == 2) cx--;
            else cy--;
        }

    } while (!(cx == static_cast<int>(x) && cy == static_cast<int>(y) && direction == 0));
}

int day12_2024(const std::string& input, std::ostream& output) {
    std::vector<std::string> grid = to_lines(input);

    size_t part1 = 0;
    size_t part2 = 0;

    for (size_t y = 0; y < grid.size(); y++) {
        for (size_t x = 0; x < grid[y].size(); x++) {
            char looking = grid[y][x];
            if (looking != '.') {
                // Now we can flood-fill and figure out the area
                size_t area = 0;
                std::vector<Location> in_region;
                {
                    std::vector<Location> stack;
                    stack.push_back({x, y});
                    while (!stack.empty()) {
                        const Location current = stack.back();
                        stack.pop_back();
                        size_t current_x = current.x;
                        size_t current_y = current.y;
                        if (grid[current_y][current_x] == '.') continue;
                        assert(grid[current_y][current_x] == looking);

                        in_region.push_back(current);

                        grid[current_y][current_x] = '.';

                        area += 1;

                        if (in_bounds(grid, current_x - 1, current_y) && grid[current_y][current_x - 1] == looking) { stack.push_back({current_x - 1, current_y}); }
                        if (in_bounds(grid, current_x + 1, current_y) && grid[current_y][current_x + 1] == looking) { stack.push_back({current_x + 1, current_y}); }
                        if (in_bounds(grid, current_x, current_y - 1) && grid[current_y - 1][current_x] == looking) { stack.push_back({current_x, current_y - 1}); }
                        if (in_bounds(grid, current_x, current_y + 1) && grid[current_y + 1][current_x] == looking) { stack.push_back({current_x, current_y + 1}); }
                    }
                }

                // Calculate the perimeter
                size_t turns = 0;
                size_t perimeter = 0;

                // We will try every location
                std::vector<Location> visited;
                for (const auto& location: in_region) {
                    // If we have already visited this wall, ignore
                   if (std::find(visited.begin(), visited.end(), location) != visited.end()) continue;
                   // If this "wall" doesn't have an acceptable space above it, ignore
                   if (std::find(in_region.begin(), in_region.end(), Location(location.x, location.y - 1)) == in_region.end()) {
                       walk(
                           grid,
                           in_region,
                           location.x, location.y,
                           perimeter,
                           turns,
                           visited
                       );
                   }
                }

                part1 += area * perimeter;
                part2 += area * turns;
            }
        }
    }

    output << "Part 1: " << part1 << std::endl; // 1485656
    output << "Part 2: " << part2 << std::endl; // 899196

    ASSERT(part1 == 1485656, part1);
    ASSERT(part2 == 899196, part2);

    return 0;
}
