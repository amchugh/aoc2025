
#include <cassert>
#include <iomanip>
#include <string>
#include <sstream>

#include "my_util.hpp"

typedef long long program_t;

// This is what the program would be in C++.
std::vector<program_t> simple(program_t a) {
    std::vector<program_t> output;

    program_t b = 0;
    program_t c = 0;

    while (a > 0) {
        // std::cout << "a: " << std::setw(6) << a << ", b: " << b << ", c: " << c << std::endl;
        b = a & 0b111;
        b = b ^ 1;         // 1,1
        // This is the tricky part.
        // Here, the largest possible
        // value of b is 7. Only the first
        // 3 bits of c end up affecting b,
        // so any number `a > 56` does not have an affect.
        c = a / (1<<b);    // 7,5
        b = b ^ c;         // 4,6
        b = b ^ 4;         // 1,4
        //output<<(b%8);   // 5,5
        output.push_back(b & 0b111);
        a = a >> 3;        // 0,3
    }

    return output;
}

std::vector<program_t> execute(
    program_t a,
    program_t b,
    program_t c,
    const std::vector<program_t>& program
) {
    std::vector<program_t> output;

    size_t ip = 0;
    while (ip < program.size() - 1) {
        assert(ip % 2 == 0);

        program_t instruction = program[ip];
        program_t literal = program[ip + 1];

        program_t *combo;
        if (literal <= 3) {
            combo = &literal;
        } else if (literal == 4) {
            combo = &a;
        } else if (literal == 5) {
            combo = &b;
        } else if (literal == 6) {
            combo = &c;
        }

        if (instruction == 0) {
            assert(literal != 7);
            a = a / (1 << *combo);
            ip += 2;
        }
        else if (instruction == 1) {
            b = b ^ literal;
            ip += 2;
        }
        else if (instruction == 2) {
            assert(literal != 7);
            b = *combo % 8;
            ip += 2;
        }
        else if (instruction == 3) {
            if (a == 0) {
                ip += 2;
            }
            else {
                ip = literal;
            }
        }
        else if (instruction == 4) {
            b ^= c;
            ip += 2;
        }
        else if (instruction == 5) {
            assert(literal != 7);
            program_t out = *combo % 8;
            output.push_back(out);
            ip += 2;
        }
        else if (instruction == 6) {
            assert(literal != 7);
            b = a / (1 << *combo);
            ip += 2;
        }
        else if (instruction == 7) {
            assert(literal != 7);
            c = a / (1 << *combo);
            ip += 2;
        }
    }
    assert(ip != program.size() - 1);

    return output;
}

bool matches(const std::vector<program_t>& goal, const std::vector<program_t>& candidate) {
    if (candidate.size() > goal.size()) return false;

    auto gitr = goal.rbegin();
    auto citr = candidate.rbegin();
    while (citr != candidate.crend()) {
        if (*gitr != *citr)
            return false;
        ++gitr;
        ++citr;
    }
    return true;
}

int day17_2024(const std::string& input, std::ostream& output) {
    std::istringstream ss(input);
    std::string _;
    std::string program_str;
    std::vector<program_t> program;

    program_t starting_a;
    program_t a;
    program_t b;
    program_t c;
    ss >> _ >> _ >> starting_a;
    a = starting_a;
    ss >> _ >> _ >> b;
    ss >> _ >> _ >> c;
    ss >> _ >> program_str;

    auto itr = program_str.begin();
    while (itr != program_str.end()) {
        program.push_back(*itr - '0');
        itr++;
        if (itr == program_str.end()) break;
        itr++;
    }

    auto part1 = execute(a, b, c, program);
    output << "Part 1: "; // 2,4,1,1,7,5,4,6,1,4,0,3,5,5,3,0
    for (size_t i = 0; i < part1.size(); ++i) {
        output << part1[i];
        if (i != part1.size() - 1) {
            output << ",";
        }
    }
    output << std::endl;

    // For part 2, we will use execute as a validator and perform a simple search.
    program_t value = 0;
    std::vector<program_t> calculated;
    while (calculated != program) {
        if (matches(program, calculated)) {
            // We can quickly advance to the next triplet
            value <<= 3;
        } else {
            value++;
        }
        calculated = execute(value, 0, 0, program);
    }
    output << "Part 2: " << value << std::endl; // 202366627359274

    return 0;
}
