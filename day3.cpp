#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>


char same_char(std::string& line) {
    auto midpoint = line.begin() + (line.length() / 2);
    return *std::find_first_of(line.begin(), midpoint, midpoint, line.end());
}

char same_in_group(std::vector<std::string>& group) {
    auto fromA = group[0].begin(), fromB = group[1].begin(), endA = group[0].end(), endB = group[1].end();
    decltype(fromA) match;

    while ((match = std::find_first_of(fromA, endA, fromB, endB)) != endA) {
        if (group[2].find(*match) != std::string::npos) {
            return *match;
        }
        fromA = match + 1;
    }
    throw "not found";
}

inline int priority(char character) {
    return ('a' <= character && character <= 'z') ? 1 + (character - 'a') : 27 + (character - 'A');
}


int main() {
    std::ifstream in{"day3.txt"};
    std::string line;
    std::vector<std::string> group;

    int wrongItemPriority = 0;
    int badgePriority = 0;

    while (std::getline(in, line)) {
        wrongItemPriority += priority(same_char(line));

        group.emplace_back(line);
        if (group.size() == 3) {
            badgePriority += priority(same_in_group(group));
            group.clear();
        }
    }

    std::cout << "Wrong item priority (a): " << wrongItemPriority << "\n";
    std::cout << "Badge priority (a): " << badgePriority << std::endl;
    return 0;
}
