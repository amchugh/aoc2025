
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <string>
#include <sstream>
#include <unordered_map>

#include "my_util.hpp"

enum operation_t {
    XOR,
    AND,
    OR,
};
char* OPERATIONS[] = {"XOR", "AND", "OR"};

struct Gate {
    std::string a;
    std::string b;
    std::string output;
    operation_t operation;
};

constexpr size_t BITS = 45;

size_t evaluate(std::unordered_map<std::string, bool> wires, std::vector<Gate> gates) {
    // We will try and evaluate gates until we consume all of them
    while (!gates.empty()) {
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
        }
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

int day24_2024(const std::string& input, std::ostream& output) {
    const auto lines = to_lines(input);

    // We know from the number of bits how many lines to read
    std::unordered_map<std::string, bool> wires;
    for (size_t i = 0; i < BITS * 2; i++) {
        auto gate_name = lines[i].substr(0, 3);
        auto status = lines[i][5] == '1';
        wires.insert({gate_name, status});
    }

    // Parse all the gates
    std::vector<Gate> gates;
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
            gates.push_back(gate);
        } catch(std::exception& ex) {
            std::cerr << "Failed to parse gate \"" << line << "\": " << ex.what() << std::endl;
            return 1;
        }
    }

    size_t part1 = evaluate(wires, gates);

    // for (const auto& wire: wires) {
    //     output << wire.first << ": " << wire.second << std::endl;
    // }
    // output << std::endl;
    // for (const auto& gate: gates) {
    //     output << gate.a << " " << OPERATIONS[gate.operation] << " " << gate.b << " -> " << gate.output << std::endl;
    // }


    size_t part2 = 0;

    output << "----------------------------------------" << std::endl;
    output << "Part 1: " << part1 << std::endl; // 42883464055378
    output << "Part 2: " << part2 << std::endl; // ???

    ASSERT(part1 == 42883464055378, part1);

    return 0;
}
