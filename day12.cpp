#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>


// Part 2 disables any heuristics, we have to use pure Dijkstra.
// Dijkstra is simplified because all edge costs are 1.

constexpr int abs(int value) {
    return value < 0 ? -value : value;
}

struct Vertex {
    int position;
    int distance;

    inline bool operator<(const Vertex &other) const {
        return distance > other.distance;
    }
};

class Field {
    std::vector<int> heights;
    int rows;
    int cols;

    int start;
    int finish;

    inline bool acceptable_direction(int from, int to) const {
        return (heights[from] - heights[to]) <= 1;
    }

    void scan(int from, auto consumer) {
        int up = from - cols;
        if (up >= 0 && acceptable_direction(from, up)) {
            consumer(up);
        }

        int down = from + cols;
        if (down < (rows * cols) && acceptable_direction(from, down)) {
            consumer(down);
        }

        int left = from - 1;
        if ((left / cols) == (from / cols) && left >= 0 && acceptable_direction(from, left)) {
            consumer(left);
        }

        int right = from + 1;
        if ((right / cols) == (from / cols) && from < (rows * cols) && acceptable_direction(from, right)) {
            consumer(right);
        }
    }

    inline Vertex pop_head(std::vector<Vertex> &open_set) {
        std::pop_heap(open_set.begin(), open_set.end());
        Vertex head = open_set.back();
        open_set.pop_back();
        return head;
    }

public:
    constexpr static int START = 0;
    constexpr static int END = 'z' - 'a' + 2;

    std::pair<int, int> find_paths() {
        std::unordered_map<int, int> distance_to_finish;
        std::vector<Vertex> open_set;

        distance_to_finish[finish] = 0;
        open_set.emplace_back(finish, 0);

        int earliest_a = -1;
        while (!open_set.empty()) {
            Vertex head = pop_head(open_set);

            if (heights[head.position] == 1 && earliest_a < 0) {
                earliest_a = head.distance;
            }

            if (head.position == start) {
                return std::pair(head.distance, earliest_a);
            }

            scan(head.position, [&](int next) {
                auto found = distance_to_finish.find(next);
                int new_distance = head.distance + 1;
                if (found == distance_to_finish.end() || new_distance < found->second) {
                    distance_to_finish[next] = new_distance;
                    open_set.emplace_back(next, new_distance);
                    std::push_heap(open_set.begin(), open_set.end());
                }
            });
        }
        throw "not found";
    }

    friend std::istream& operator>>(std::istream &in, Field &field);
};


std::istream& operator>>(std::istream &in, Field &field) {
    std::string line;
    std::getline(in, line);

    auto to_int = [](char ch) {
        if (ch == 'S') {
            return Field::START;
        } else if (ch == 'E') {
            return Field::END;
        } else {
            return ch - 'a' + 1;
        }
    };

    field.cols = line.size();
    field.rows = 1;
    field.heights.clear();

    do {
        std::transform(line.cbegin(), line.cend(), std::back_inserter(field.heights), to_int);
        field.rows += 1;
    } while (std::getline(in, line));

    auto begin = field.heights.begin();
    auto end = field.heights.end();
    field.start = int(std::find(begin, end, Field::START) - begin);
    field.finish = int(std::find(begin, end, Field::END) - begin);
    return in;
}


int main() {
    std::ifstream in("day12.txt");
    Field field;
    in >> field;

    auto pair = field.find_paths();
    std::cout << "Distance to start (a): " << pair.first << "\n";
    std::cout << "Distance to a (b): " << pair.second << "\n";
    return 0;
}
