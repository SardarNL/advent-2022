#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <regex>
#include <vector>
#include <limits>
#include <utility>
#include <stdexcept>


const std::regex CRATE ("(?:\\[([A-Z])\\]|\\s{3})\\s?");
const std::regex END_OF_COLUMNS ("(?:\\s+\\d+)+\\s*");
const std::regex MOVE_LINE("move (\\d+) from (\\d+) to (\\d+)");


class MoveCommand {
    int from;
    int to;
    int count;

public:
    inline void set(int fromV, int toV, int countV) {
        from = fromV - 1;
        to = toV - 1;
        count = countV;
    }

    friend class Board;
};

class Board {
    std::vector<std::string> columns;

    void move_with(const MoveCommand &move, auto &&functor) {
        auto &from = columns.at(move.from);
        auto &to = columns.at(move.to);
        if (move.count > from.length()) {
            throw std::out_of_range("should not happen");
        }

        functor(from, to, move.count);
        from.resize(from.length() - move.count);
    }

public:
    Board(const std::vector<std::string> &board) : columns(board) {}
    Board(std::vector<std::string> &&board) : columns(std::move(board)) {}

    void move_single(const MoveCommand &move) {
        move_with(move, [](auto &from, auto &to, int count) {
            to.append(from.rbegin(), from.rbegin() + count);
        });
    }

    void move_multi(const MoveCommand &move) {
        move_with(move, [](auto &from, auto &to, int count) {
            to.append(from.end() - count, from.end());
        });
    }

    std::string top() const {
        std::string result;
        std::for_each(columns.begin(), columns.end(), [&result](auto&& column) {
            if (column.empty()) {
                result += '*';
            } else {
                result += column.back();
            }
        });

        return result;
    }
};


std::istream& operator>>(std::istream &in, MoveCommand &item) {
    std::string line;
    std::getline(in, line);

    std::smatch match;
    if (std::regex_match(line, match, MOVE_LINE)) {
        item.set(std::stoi(match[2]), std::stoi(match[3]), std::stoi(match[1]));
    }
    return in;
}

std::vector<std::string> parseStacks(std::ifstream &in) {
    std::string line;
    std::getline(in, line);
    std::vector<std::string> board((line.length() + 1) / 4);
    
    do {
        std::sregex_iterator from(line.begin(), line.end(), CRATE), to;
        auto boardFrom = board.begin();
        for (; from != to; ++from, ++boardFrom) {
            if (from->length(1) != 0) {
                boardFrom->append((*from)[1].str());
            }
        }
    } while (std::getline(in, line) && !std::regex_match(line, END_OF_COLUMNS));

    std::for_each(board.begin(), board.end(), [](auto&& line) {
        std::reverse(line.begin(), line.end());
    });
    return board;
}


int main() {
    std::ifstream in{"day5.txt"};

    auto stacks = parseStacks(in);
    Board single(stacks);
    Board multi(std::move(stacks));

    in >> std::ws;
    std::istream_iterator<MoveCommand> from{in}, to;
    std::for_each(from, to, [&single, &multi](auto&& command) {
        single.move_single(command);
        multi.move_multi(command);
    });

    std::cout << "Single (a): " << single.top() << "\n";
    std::cout << "Multi (b): " << multi.top() << "\n";
    return 0;
}
