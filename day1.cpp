#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <numeric>


class Elf {
    int callories = 0;

public:
    Elf& operator=(int newCallories) {
        callories = newCallories;
        return *this;
    }

    operator int() const {
        return callories;
    }
};

std::istream& operator>>(std::istream& is, Elf& elf) {
    std::string line;
    int value = 0;

    while (std::getline(is, line) && !line.empty()) {
        value += std::stoi(line);
    }
    
    elf = value;
    return is;
}


int main() {
    std::ifstream in {"day1.txt"};
    std::istream_iterator<Elf> from{in}, to;
    std::vector<int> callories{from, to};
    
    std::partial_sort(begin(callories), begin(callories) + 3, end(callories), std::greater<>());

    std::cout << "Max callories: " << callories[0] << "\n";
    std::cout << "Top three: " << std::accumulate(begin(callories), begin(callories) + 3, 0) << std::endl;
    return 0;
}
