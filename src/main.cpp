
#include <exception>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <fstream>

#include "my_util.h"

int day1(const std::string&, std::ostream&);
int day2(const std::string&, std::ostream&);
int day3(const std::string&, std::ostream&);
int day4(const std::string&, std::ostream&);

int profile_all_days(auto target_duration) {
    constexpr size_t NUM_DAYS = 4;
    std::string* inputs = new std::string[NUM_DAYS];
    for (size_t i = 0; i < NUM_DAYS; i++) {
        inputs[i] = read_file_as_string(std::format("resources/day{}.txt", i + 1));
    }

    std::ofstream quiet_output;
    quiet_output.setstate(std::ios_base::badbit);

    std::vector<std::chrono::microseconds> times;
    std::chrono::microseconds duration;

    std::cout << "Running for " << target_duration << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    do {
        auto local_start = std::chrono::high_resolution_clock::now();

        day1(inputs[0], quiet_output);
        day2(inputs[1], quiet_output);
        day3(inputs[2], quiet_output);
        day4(inputs[3], quiet_output);

        auto end = std::chrono::high_resolution_clock::now();

        auto run_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - local_start);
        times.push_back(run_duration);

        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    } while (duration < target_duration);

    // Calculate and print statistics
    std::sort(times.begin(), times.end());
    auto min_time = times.front();
    auto max_time = times.back();
    auto median_time = times[times.size() / 2];

    long long total = 0;
    for (const auto& t : times) {
        total += t.count();
    }
    auto avg_time = std::chrono::microseconds(total / times.size());

    const size_t iterations = times.size();
    std::cout << "Benchmark (" << iterations << " runs in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration) << "):" << std::endl;
    std::cout << "  Min: " << min_time.count() / 1000. << "ms" << std::endl;
    std::cout << "  Max: " << max_time.count() / 1000. << "ms" << std::endl;
    std::cout << "  Avg: " << avg_time.count() / 1000. << "ms" << std::endl;
    std::cout << "  Median: " << median_time.count() / 1000. << "ms" << std::endl;

    delete[] inputs;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc >= 2 && argv[1] == std::string("all")) {
        if(argc != 3) {
            std::cerr << "Invalid arguments. " << argv[0] << " all <timing_duration_seconds>" << std::endl;
            return 1;
        }
        int val = atoi(argv[2]);
        if (val <= 0) {
            std::cerr << "Bad duration argument \"" << argv[2] << "\" (read as " << val << "). Must be a positive integer." << std::endl;
            return 1;
        }
        auto target_duration = std::chrono::seconds(val);

        return profile_all_days(target_duration);
    }

    if (argc != 2 && argc != 3 && argc != 4) {
        std::cerr << "Invalid arguments. " << argv[0] << " <day_number> [input_file_path] [timing_duration_seconds]" << std::endl;
        return 1;
    }

    const int day_number = atoi(argv[1]);

    std::string input;
    if (argc == 2) {
        // Read from default
        std::string path = std::format("resources/day{}.txt", day_number);
        input = read_file_as_string(path);
    } else {
        input = read_file_as_string(std::string(argv[2]));
    }

    int (*fn)(const std::string&, std::ostream&);
    if (day_number == 1) fn = day1;
    else if (day_number == 2) fn = day2;
    else if (day_number == 3) fn = day3;
    else if (day_number == 4) fn = day4;
    else {
        std::cerr << "Invalid arguments. Day " << day_number << " is not a valid day." << std::endl;
        return 1;
    }

    // Run it for a period and time it
    std::chrono::seconds target_duration;
    if (argc == 4) {
        try {
            int val = atoi(argv[3]);
            if (val <= 0) {
                std::cerr << "Bad duration argument \"" << argv[3] << "\" (read as " << val << "). Must be a positive integer." << std::endl;
                return 1;
            }
            target_duration = std::chrono::seconds(val);
        } catch (std::exception& ex) {
            std::cerr << "Failed to read duration argument " << argv[3] << ": " << ex.what() << std::endl;
            return 1;
        }
    }
    else {
        target_duration = std::chrono::seconds(5);
    }

    std::vector<std::chrono::microseconds> times;

    std::ofstream quiet_output;
    quiet_output.setstate(std::ios_base::badbit);

    std::ostream* current_output = &std::cout;

    std::chrono::microseconds duration;

    std::cout << "Running for " << target_duration << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    do {
        auto local_start = std::chrono::high_resolution_clock::now();
        int result = fn(input, *current_output);
        auto end = std::chrono::high_resolution_clock::now();
        if (result != 0) return result;

        auto run_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - local_start);
        times.push_back(run_duration);
        current_output = &quiet_output;

        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    } while (duration < target_duration);

    // Calculate and print statistics
    std::sort(times.begin(), times.end());
    auto min_time = times.front();
    auto max_time = times.back();
    auto median_time = times[times.size() / 2];

    long long total = 0;
    for (const auto& t : times) {
        total += t.count();
    }
    auto avg_time = std::chrono::microseconds(total / times.size());

    const size_t iterations = times.size();
    std::cout << "Benchmark (" << iterations << " runs in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration) << "):" << std::endl;
    std::cout << "  Min: " << min_time.count() / 1000. << "ms" << std::endl;
    std::cout << "  Max: " << max_time.count() / 1000. << "ms" << std::endl;
    std::cout << "  Avg: " << avg_time.count() / 1000. << "ms" << std::endl;
    std::cout << "  Median: " << median_time.count() / 1000. << "ms" << std::endl;

    return 0;
}
