#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <tuple>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <limits>
#include <vector>


struct Vertex {
    int x;
    int y;

    inline bool contains(int pos) const {
        return x <= pos && pos <= y;
    }

    operator int() const {
        return (x << 16) | y;
    }

    auto operator<=>(const Vertex &other) const = default;
};


class Wall {
    std::map<int, std::map<int, int>> lines;

public:
    void add_wall(int at, int _from, int _to) {
        int from, to;
        std::tie(from, to) = std::minmax(_from, _to);

        auto &line = lines[at];
        auto sibling = line.lower_bound(to);
        if (sibling == line.end() || sibling->second > to) {
          line[to] = from;
        } else {
            sibling->second = std::min(sibling->second, from);
        }
    }

    bool collide(int x, int y) const {
        auto found = lines.find(y);
        if (found == lines.end()) {
            return false;
        }

        auto range = found->second.lower_bound(x);
        return range != found->second.end() && range->second <= x;
    }
};

class Cave {
    Wall horizontal;
    Wall vertical;
    int floor = -1;
    std::unordered_set<int> grains;
    std::vector<Vertex> path;

    inline bool is_allowed(const Vertex &grain) const {
        return grain.y != floor && !grains.contains(grain) && !horizontal.collide(grain.x, grain.y) && !vertical.collide(grain.y, grain.x);
    }

    inline bool fall_down(Vertex &grain, int offset_x) const {
        Vertex next(grain.x + offset_x, grain.y + 1);
        if (is_allowed(next)) {
            grain = next;
            return true;
        }
        return false;
    }

    bool simulate_without_floor() {
        Vertex grain = path.back();
        while (fall_down(grain, 0) || fall_down(grain, -1) || fall_down(grain, 1)) {
            path.emplace_back(grain);
        }
        grains.emplace(grain);
        if (grain.y + 1 == floor) {
            return false;
        }
        path.pop_back();
        return true;
    }

    bool simulate_with_floor() {
        if (path.empty()) {
            return false;
        }
        Vertex grain = path.back();
        while (fall_down(grain, 0) || fall_down(grain, -1) || fall_down(grain, 1)) {
            path.emplace_back(grain);
        }
        path.pop_back();
        grains.emplace(grain);
        return true;
    }

public:
    void add_wall(Vertex from, Vertex to) {
        if (from.x - to.x == 0) {
            vertical.add_wall(from.x, from.y, to.y);
        } else {
            horizontal.add_wall(from.y, from.x, to.x);
        }
        floor = std::max(floor, std::max(from.y, to.y) + 2);
    }

    std::pair<int, int> simulate_grain(Vertex grain) {
        path.emplace_back(grain);
        
        int rested_without_floor = 0;
        while (simulate_without_floor()) {
            rested_without_floor += 1;
        }

        int rested_with_floor = rested_without_floor;
        while (simulate_with_floor()) {
            rested_with_floor += 1;
        }

        return { rested_without_floor, rested_with_floor };
    }

    friend std::istream& operator>>(std::istream &in, Cave &cave);
};

Vertex read_vertex(std::istringstream &row) {
    Vertex ret;
    row >> ret.x;
    row.ignore(1);
    row >> ret.y;
    row.ignore(4);
    return ret;
};

std::istream& operator>>(std::istream &in, Cave &cave) {
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream row(line);
        auto prev = read_vertex(row);
        while (row) {
            auto next = read_vertex(row);
            cave.add_wall(prev, next);
            prev = next;
        }
    }
    return in;
}

int main() {
    std::ifstream in("day14.txt");
    Cave cave;
    in >> cave;

    Vertex emitter(500, 0);
    auto [rested_without_floor, rested_with_floor] = cave.simulate_grain(emitter);

    std::cout << "No floor (a): " << rested_without_floor << "\n";
    std::cout << "With floor (b): " << rested_with_floor << "\n";
    return 0;
}
