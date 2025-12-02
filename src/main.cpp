
#include <iostream>

#include "my_util.h"

int day1(const std::string&);
int day2(const std::string&);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Invalid arguments. " << argv[0] << " <day_number> <input_file_path>" << std::endl;
        return 1;
    }

    const int day_number = atoi(argv[1]);
    const auto input = to_string(std::string(argv[2]));

    if (day_number == 1) return day1(input);
    if (day_number == 2) return day2(input);

    std::cerr << "Invalid arguments. Day " << day_number << " is not a valid day." << std::endl;
    return 1;
}
