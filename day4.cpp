#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>


class Interval : public std::pair<int, int> {
    using std::pair<int, int>::pair;

public:
    inline bool contains(const Interval &other) const {
        return first <= other.first && other.second <= second;
    }

    inline bool overlaps(const Interval &other) const {
        return !(second < other.first || first > other.second);
    }
};

class Group : public std::pair<Interval, Interval> {
    using std::pair<Interval, Interval>::pair;

public:
    inline bool is_fully_contained() const {
        return first.contains(second) || second.contains(first);
    }

    inline bool is_overlap() const {
        return first.overlaps(second);
    }
};

template <class Pair>
std::istream& operator>> (std::istream& stream, Pair& item) {
    stream >> item.first;
    stream.ignore(1);
    stream >> item.second;
    return stream;
}


int main() {
    std::ifstream in{"day4.txt"};
    std::istream_iterator<Group> from{in}, to;

    int fully_contained = 0;
    int overlap = 0;
    std::for_each(from, to, [&](auto group) {
        fully_contained += group.is_fully_contained();
        overlap += group.is_overlap();
    });

    std::cout << "Fully contained (a): " << fully_contained << "\n";
    std::cout << "Overlap (b): " << overlap << std::endl;
    return 0;
}
