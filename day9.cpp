#include <iostream>
#include <fstream>
#include <iterator>
#include <set>
#include <algorithm>
#include <vector>

constexpr int sign(int value) {
    return value < 0 ? -1: 1;
}

constexpr int abs(int value) {
    return value < 0 ? -value : value;
}

struct Position {
    int x = 0;
    int y = 0;

    Position direction() const {
        if (x != 0) {
            return { sign(x), 0 };
        } else {
            return { 0, sign(y) };
        }
    }

    Position& operator+=(const Position& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Position operator+(const Position& other) {
        return { x + other.x, y + other.y };
    }

    auto operator<=>(const Position& rhs) const = default;

    friend class Rope;
};


class Rope {
    std::vector<Position> knots;

    void propagate() {
        for (auto head = knots.begin(), tail = head + 1; tail != knots.end(); ++head, ++tail) {
            int horizontal = head->x - tail->x;
            int vertical = head->y - tail->y;
            
            if (abs(horizontal) == 2 && abs(vertical) == 2) {
                tail->x += sign(horizontal);
                tail->y += sign(vertical);
            } else if (abs(horizontal) == 2) {
                tail->x = head->x - sign(horizontal);
                tail->y = head->y;
            } else if (abs(vertical) == 2) {
                tail->x = head->x;
                tail->y = head->y - sign(vertical);
            }
        }
    }

public:
    Rope(int segments) : knots(segments) {}

    void move(const Position& shift, auto inserterFirst, auto inserterLast) {
        auto dir = shift.direction();
        auto to = knots.front() + shift;
        while (knots.front() != to) {
            knots.front() += dir;
            propagate();
            *inserterFirst++ = knots[1];
            *inserterLast++ = knots.back();
        }
    }
};

std::istream& operator>>(std::istream& in, Position& shift) {
    char move;
    int offset;
    in >> move >> offset;
    shift.x = move == 'R' ? offset : (move == 'L' ? -offset : 0);
    shift.y = move == 'U' ? offset : (move == 'D' ? -offset : 0);
    return in;
}


int main() {
    std::ifstream in("day9.txt");
    std::istream_iterator<Position> from{in}, to;
    Rope rope(10);

    std::set<Position> seenFirst;
    std::set<Position> seenLast;
    auto inserterFirst =  std::inserter(seenFirst, seenFirst.begin());
    auto inserterLast =  std::inserter(seenLast, seenLast.begin());

    std::for_each(from, to, [&rope, &inserterFirst, &inserterLast](const Position& shift) {
        rope.move(shift, inserterFirst, inserterLast);
    });

    std::cout << "First knot (a): " << seenFirst.size() << "\n";
    std::cout << "Last knot (b): " << seenLast.size() << std::endl;
    return 0;
}
