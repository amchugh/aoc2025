
#include <algorithm>
#include <cassert>
#include <cstring>
#include <format>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "my_util.hpp"

enum operation_t {
    XOR,
    AND,
    OR,
};
const char* OPERATIONS[] = {"XOR", "AND", "OR"};

struct Gate {
    std::string a;
    std::string b;
    std::string output;
    operation_t operation;
};

constexpr size_t BITS = 45;

unsigned long evaluate(std::unordered_map<std::string, bool> wires, std::vector<Gate> gates) {
    // We will try and evaluate gates until we consume all of them
    size_t evaluated = 0;
    while (!gates.empty()) {
        evaluated = 0;
        auto itr = gates.begin();
        while (itr != gates.end()) {
            auto& gate = *itr;
            if (!wires.contains(gate.a) || !wires.contains(gate.b)) {
                ++itr;
                continue;
            }

            auto a = wires[gate.a];
            auto b = wires[gate.b];
            auto output = gate.output;

            switch (gate.operation) {
                case AND:
                    wires[output] = a && b;
                    break;
                case OR:
                    wires[output] = a || b;
                    break;
                case XOR:
                    wires[output] = a != b;
                    break;
            }

            itr = gates.erase(itr);
            evaluated++;
        }
        if (evaluated == 0)
            throw std::runtime_error("No gates evaluated");
    }

    // Now we construct the output
    size_t result = 0;
    for (int idx = BITS; idx >= 0; --idx) {
        std::string wire_name = std::format("z{:02}", idx);
        assert(wires.contains(wire_name));
        result = (result << 1) | wires[wire_name];
    }

    return result;
}

std::unordered_map<std::string, bool> numbers_to_gates(unsigned long x, unsigned long y) {
    std::unordered_map<std::string, bool> gates;
    for (size_t idx = 0; idx < BITS; idx++) {
        std::string wire_name_x = std::format("x{:02}", idx);
        gates[wire_name_x] = (x >> idx) & 1;

        std::string wire_name_y = std::format("y{:02}", idx);
        gates[wire_name_y] = (y >> idx) & 1;
    }
    assert(gates.size() == BITS * 2);
    return gates;
}

int day24_2024(const std::string& input, std::ostream& output) {
    const auto lines = to_lines(input);

    // We know from the number of bits how many lines to read
    std::unordered_map<std::string, bool> wires;
    for (size_t i = 0; i < BITS * 2; i++) {
        auto gate_name = lines[i].substr(0, 3);
        auto status = lines[i][5] == '1';
        wires.emplace(gate_name, status);
    }

    // Parse all the gates
    std::vector<Gate> gates;
    std::unordered_map<std::string, size_t> output_to_index;
    for (size_t i = BITS * 2 + 1; i < lines.size(); i++) {
        const auto& line = lines[i];
        Gate gate;
        try {
            gate.a = line.substr(0, 3);
            if (line[4] == 'O') {
                gate.operation = OR;
                gate.b = line.substr(7, 3);
                gate.output = line.substr(14, 3);
            } else {
                gate.operation = line[4] == 'A' ? AND : XOR;
                gate.b = line.substr(8, 3);
                gate.output = line.substr(15, 3);
            }
            output_to_index.emplace(gate.output, gates.size());
            gates.push_back(gate);
        } catch(std::exception& ex) {
            std::cerr << "Failed to parse gate \"" << line << "\": " << ex.what() << std::endl;
            return 1;
        }
    }

    auto part1 = evaluate(wires, gates);

    // Start with all 1s
    {
        unsigned long x = 0b111111111111111111111111111111111111111111111;
        unsigned long y = 0b111111111111111111111111111111111111111111111;
        unsigned long expected = x + y;
        unsigned long actual = evaluate(numbers_to_gates(x, y), gates);
        std::cerr << "x:        " << std::bitset<64>(x) << std::endl;
        std::cerr << "y:        " << std::bitset<64>(y) << std::endl;
        std::cerr << "expected: " << std::bitset<64>(expected) << std::endl;
        std::cerr << "actual:   " << std::bitset<64>(actual) << std::endl;
        unsigned long difference = expected ^ actual;
        std::cerr << "incorrect:" << std::bitset<64>(difference) << std::endl;

        unsigned long looking = difference;
        std::unordered_set<std::string> involved;
        while (looking) {
            size_t first_one_idx = __builtin_ctzll(looking);
            looking ^= 1 << first_one_idx;
            std::string gate_name = std::format("z{:02}", first_one_idx);
            involved.insert(gate_name);
        }

        std::unordered_set<std::string> prior_wires;
        while (prior_wires.size() != involved.size()) {
            prior_wires = involved;
            // Find any gates with output wires in involved
            for (const auto& gate : gates) {
                if (std::find(involved.begin(), involved.end(), gate.output) != involved.end()) {
                    involved.insert(gate.a);
                    involved.insert(gate.b);
                }
            }
        }

        // We can remove all of the input gates
        auto itr = involved.begin();
        while (itr != involved.end()) {
            if (itr->starts_with("x") || itr->starts_with("y")) {
                itr = involved.erase(itr);
            } else {
                ++itr;
            }
        }

        std::cerr << "involved(" << involved.size() << "): ";
        for (const auto& gate_name : involved) {
            std::cerr << gate_name << " ";
        }
        std::cerr << std::endl;

        // Let's go through every wire and see if we can swap it with
        // an other such that the input does _not_ change.
        for (auto itr = involved.begin(); itr != involved.end(); ) {
            for (auto later = itr++; later != involved.end(); ++later) {
                // Swap the output gates
                gates[output_to_index[*itr]].output = *later;
                gates[output_to_index[*later]].output = *itr;

                try {
                    unsigned long new_actual = evaluate(numbers_to_gates(x, y), gates);
                    if (actual == new_actual) {
                        output << "Gates " << *itr << " and " << *later << " did not change anything" << std::endl;
                    } else {
                        output << "Gates " << *itr << " and " << *later << " changed something: " << std::bitset<64>(new_actual) << std::endl;
                    }
                    // unsigned long difference = expected ^ actual;
                    // std::cerr << "incorrect:" << std::bitset<64>(difference) << std::endl;
                } catch (const std::runtime_error& e) {
                    // std::cerr << "error: " << e.what() << std::endl;
                }

                // Swap them back
                gates[output_to_index[*itr]].output = *itr;
                gates[output_to_index[*later]].output = *later;
            }
        }
    }

    // Let's do adjacent pairs until we find a discrepency
    // for (size_t x_idx = 0; x_idx < BITS - 1; x_idx++) {
    //     unsigned long x = 0b11 << x_idx;
    //     for (size_t y_idx = 0; y_idx < BITS - 1; y_idx++) {
    //         unsigned long y = 0b11 << y_idx;
    //         unsigned long expected = x + y;
    //         unsigned long actual = evaluate(numbers_to_gates(x, y), gates);
    //         if (actual != expected) {
    //             std::cerr << "Discrepancy found at (" << x_idx << ", " << y_idx << ")" << std::endl;
    //             // Print the bits of x, y, expeceted, and actual
    //             std::cerr << "x:        " << std::bitset<64>(x) << std::endl;
    //             std::cerr << "y:        " << std::bitset<64>(y) << std::endl;
    //             std::cerr << "expected: " << std::bitset<64>(expected) << std::endl;
    //             std::cerr << "actual:   " << std::bitset<64>(actual) << std::endl;
    //             unsigned long difference = expected ^ actual;
    //             std::cerr << "incorrect:" << std::bitset<64>(difference) << std::endl;

    //             // Get the index of the first 1 in the difference
    //             size_t first_one_index = __builtin_ctzll(difference);
    //             std::cerr << "first_one_index: " << first_one_index << std::endl;

    //             // Get the number of incorrect bits (the number of 1s in the difference)
    //             size_t num_incorrect_bits = __builtin_popcountll(difference);
    //             std::cerr << "num_incorrect_bits: " << num_incorrect_bits << std::endl;

    //             return 1;
    //         }
    //     }
    // }


    size_t part2 = 0;

    output << "Part 1: " << part1 << std::endl; // 42883464055378
    output << "Part 2: " << part2 << std::endl; // ???

    ASSERT(part1 == 42883464055378, part1);

    return 1;
}
