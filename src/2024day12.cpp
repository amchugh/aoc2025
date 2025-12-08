
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

int safe_matches(const std::vector<std::string>& grid, int x, int y, char looking) {
    return in_bounds(grid, x, y) && grid[y][x] == looking;
}

typedef char direction_t;
// 0 = right, 1 = down, 2 = left, 3 = up

struct Location {
    size_t x;
    size_t y;
};

struct Region {
    size_t area;
    size_t perimeter;
    size_t turns;
    char repr;
};

void walk(const std::vector<std::string>& grid, int cx, int cy, char looking, size_t& perimeter, size_t& turns);

int day12_2024(const std::string& input, std::ostream& output) {
    output << input << std::endl;

    std::vector<std::string> grid = to_lines(input);

    const size_t grid_width = grid[0].size();
    const size_t grid_area = grid.size() * grid_width;
    size_t region_map[grid_area];
    std::vector<Region> regions;

    // Fill the region map with -1s
    for (size_t i = 0; i < grid_area; i++) {
        region_map[i] = -1;
    }

    for (size_t y = 0; y < grid.size(); y++) {
        for (size_t x = 0; x < grid[y].size(); x++) {
            char looking = grid[y][x];
            if (looking != '.') {
                // .>..
                // .OO.
                // ..O.
                // ....

                // ..>.
                // .OO.
                // ..O.
                // ....

                // ....
                // .>O.
                // .OO.
                // ....

                // Now that we have the area, we will calculate the perimeter
                size_t turns = 0;
                size_t perimeter = 0;

                // We will by default be moving right. We always move clockwise.
                direction_t direction = 0;

                int cx = x;
                int cy = y;

                do {
                    assert(grid[cy][cx] == looking);
                    perimeter++;

                    int inside_ahead_x = cx;
                    int inside_ahead_y = cy;

                    if (direction == 0) { inside_ahead_x++; }
                    else if (direction == 1) { inside_ahead_y++; }
                    else if (direction == 2) { inside_ahead_x--; }
                    else { inside_ahead_y--; }

                    int outside_ahead_x = cx;
                    int outside_ahead_y = cy;

                    if (direction == 0) { outside_ahead_x++; outside_ahead_y--; }
                    else if (direction == 1) { outside_ahead_y++; outside_ahead_x++; }
                    else if (direction == 2) { outside_ahead_x--; outside_ahead_y++; }
                    else { outside_ahead_y--; outside_ahead_x--; }

                    // Is there a wall in our way?
                    if (safe_matches(grid, outside_ahead_x, outside_ahead_y, looking)) {
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
                    else if (!safe_matches(grid, inside_ahead_x, inside_ahead_y, looking)) {
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

                } while (!(cx == x && cy == y && direction == 0));

                // While we do this flood fill, discover if we are surrounded by the same idx everywhere
                size_t surrounded = -1;
                bool is_surrounded = true;

                // Now we can flood-fill and figure out the area
                size_t area = 0;
                std::vector<Location> stack;
                stack.push_back({x, y});
                while (!stack.empty()) {
                    const Location current = stack.back();
                    stack.pop_back();
                    size_t current_x = current.x;
                    size_t current_y = current.y;
                    if (grid[current_y][current_x] == '.') continue;
                    assert(grid[current_y][current_x] == looking);

                    grid[current_y][current_x] = '.';
                    region_map[current_y * grid_width + current_x] = regions.size();
                    assert(surrounded != regions.size());

                    area += 1;

                    if (in_bounds(grid, current_x - 1, current_y)) {
                        if (grid[current_y][current_x - 1] == looking) { stack.push_back({current_x - 1, current_y}); }
                        else {
                            if (surrounded == static_cast<size_t>(-1) && region_map[current_y * grid_width + current_x - 1] != regions.size()) { surrounded = region_map[current_y * grid_width + current_x - 1]; }
                            else { is_surrounded = is_surrounded && region_map[current_y * grid_width + current_x - 1] == surrounded; }
                        }
                    } else { is_surrounded = false; }
                    if (in_bounds(grid, current_x + 1, current_y)) {
                        if (grid[current_y][current_x + 1] == looking) { stack.push_back({current_x + 1, current_y}); }
                        else {
                            if (surrounded == static_cast<size_t>(-1) && region_map[current_y * grid_width + current_x + 1] != regions.size()) { surrounded = region_map[current_y * grid_width + current_x + 1]; }
                            else { is_surrounded = is_surrounded && region_map[current_y * grid_width + current_x + 1] == surrounded; }
                        }
                    } else { is_surrounded = false; }
                    if (in_bounds(grid, current_x, current_y - 1)) {
                        if (grid[current_y - 1][current_x] == looking) { stack.push_back({current_x, current_y - 1}); }
                        else {
                            if (surrounded == static_cast<size_t>(-1) && region_map[(current_y - 1) * grid_width + current_x] != regions.size()) { surrounded = region_map[(current_y - 1) * grid_width + current_x]; }
                            else { is_surrounded = is_surrounded && region_map[(current_y - 1) * grid_width + current_x] == surrounded; }
                        }
                    } else { is_surrounded = false; }
                    if (in_bounds(grid, current_x, current_y + 1)) {
                        if (grid[current_y + 1][current_x] == looking) { stack.push_back({current_x, current_y + 1}); }
                        else {
                            if (surrounded == static_cast<size_t>(-1) && region_map[(current_y + 1) * grid_width + current_x] != regions.size()) { surrounded = region_map[(current_y + 1) * grid_width + current_x]; }
                            else { is_surrounded = is_surrounded && region_map[(current_y + 1) * grid_width + current_x] == surrounded; }
                        }
                    } else { is_surrounded = false; }
                }

                if (is_surrounded) {
                    // We need to go update the region we surround with our turns and perimeter as it encloses us
                    assert(surrounded != static_cast<size_t>(-1));
                    regions[surrounded].perimeter += perimeter;
                    regions[surrounded].turns += turns;
                }

                // Add the region
                regions.push_back({.area = area, .perimeter = perimeter, .turns = turns, .repr = looking});
            }
        }
    }

    size_t part1 = 0;
    size_t part2 = 0;
    for (const auto& region : regions) {
        output << "A region of " << region.repr << " plants with price " << region.area << " * " << region.perimeter << " = " << region.area * region.perimeter << std::endl;
        part1 += region.area * region.perimeter;
        part2 += region.area * region.turns;
    }

    output << "Part 1: " << part1 << std::endl; // 1485656
    output << "Part 2: " << part2 << std::endl;
    // 894924 -- too low

    ASSERT(part1 == 1485656, part1);

    return 1;
}
