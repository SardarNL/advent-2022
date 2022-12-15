#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string_view>

// [ [ -> consume, result = 0
// , , -> consume, result = 0
// D D -> consume, result D-D
// ] ] -> consume, result = 0
// E E -> result = 0, end
//
// * E -> result 1, end
// E * -> result -1, end
//
// D [ -> pop left D, push left ], push left D, push left [
// [ D -> pop right D, push right ], push right D, push right [
//
// ] D -> result -1
// D ] -> result 1
//
// ] , -> result -1, end
// , ] -> result 1, end


constexpr bool is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}

class Pairwise {
    std::string left;
    std::string right;

    using pos = std::string::iterator;

    int compare(pos &from_left, pos &from_right) {
        for (;;) {
            if (from_left == left.end() && from_right == right.end()) {
                return 0;
            }
            if (from_left == left.end()) {
                return -1;
            }
            if (from_right == right.end()) {
                return 1;
            }

            if (!is_digit(*from_left) && *from_left == *from_right) {
                ++from_left;
                ++from_right;
                continue;
            }
            if (is_digit(*from_left) && is_digit(*from_right)) {
                int result = consume_numbers(from_left, from_right);
                if (result != 0) {
                    return result;
                }
                continue;
            }
            if (is_digit(*from_left) && *from_right == '[') {
                to_list(from_left, left);
                continue;
            }
            if (*from_left == '[' && is_digit(*from_right)) {
                to_list(from_right, right);
                continue;
            }
            if (*from_left == ']') {
                return -1;
            }
            if (*from_right == ']') {
                return 1;
            }
            throw "should not happen";
        }
        return 0;
    }

    int to_int(pos &from, std::string &line) {
        std::size_t consumed;
        int num = std::stoi(line.substr(from - line.begin(), 5), &consumed);
        from += consumed;
        return num;
    }

    int consume_numbers(pos &from_left, pos &from_right) {
        return to_int(from_left, left) - to_int(from_right, right);
    }
    
    void to_list(pos &from, std::string &line) {
        int num = to_int(from, line);
        std::ostringstream replace;
        replace << '[' << num << ']';
        line.replace(line.begin(), from, replace.str());
        from = line.begin();
    }

public:
    Pairwise(const std::string &_left, const std::string &_right) : left(_left), right(_right) { }

    int compare() {
        pos from_left = left.begin();
        pos from_right = right.begin();

        return compare(from_left, from_right);
    }
};

class Packet {

public:
    std::string line;

    Packet(const std::string &_line) : line(_line) { }

    bool operator<(const Packet &other) const {
        Pairwise pairs(line, other.line);
        return pairs.compare() < 0;
    }

    bool operator==(const std::string_view &other) {
        return line == other;
    }
};


int main() {
    std::ifstream in("day13.txt");
    std::vector<Packet> packets;

    int pair_index = 0;
    int sum_of_indices = 0;
    do {
        pair_index += 1;
        std::string left;
        std::string right;
        std::getline(in, left);
        std::getline(in, right);
        in >> std::ws;

        if (!left.empty() && !right.empty()) {
            Pairwise pairs(left, right);
            if (pairs.compare() < 0) {
                sum_of_indices += pair_index;
            }
            packets.emplace_back(left);
            packets.emplace_back(right);
        }
    } while (in);

    packets.emplace_back("[[2]]");
    packets.emplace_back("[[6]]");
    std::sort(packets.begin(), packets.end());

    auto begin = std::find(packets.begin(), packets.end(), "[[2]]") - packets.begin() + 1;
    auto end = std::find(packets.begin(), packets.end(), "[[6]]") - packets.begin() + 1;

    std::cout << "Correct pairs (a): " << sum_of_indices << "\n";
    std::cout << "Decoder key (b): " << (begin * end) << "\n";
    return 0;
}
