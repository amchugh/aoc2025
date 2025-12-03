
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

#include "my_util.h"

int day1(const std::string&, std::ostream&);
int day2(const std::string&, std::ostream&);
int day3(const std::string&, std::ostream&);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Invalid arguments. " << argv[0] << " <day_number> <input_file_path>" << std::endl;
        return 1;
    }

    const int day_number = atoi(argv[1]);
    const auto input = to_string(std::string(argv[2]));

    int (*fn)(const std::string&, std::ostream&);
    if (day_number == 1) fn = day1;
    else if (day_number == 2) fn = day2;
    else if (day_number == 3) fn = day3;
    else {
        std::cerr << "Invalid arguments. Day " << day_number << " is not a valid day." << std::endl;
        return 1;
    }

    // Run it for a period and time it
    auto target_duration = std::chrono::seconds(5);
    std::vector<std::chrono::microseconds> times;

    auto start = std::chrono::high_resolution_clock::now();

    std::ofstream quiet_output;
    quiet_output.setstate(std::ios_base::badbit);

    std::ostream* current_output = &std::cout;

    std::chrono::microseconds duration;

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
