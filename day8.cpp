#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <ranges>
#include <utility>
#include <string>
#include <numeric>
#include <algorithm>



template<class It>
class striding_iterator {
    It iter;
    int stride;

public:
    using iterator_category = It::iterator_category;
    using value_type = It::value_type;
    using difference_type = It::difference_type;
    using pointer = It::pointer;
    using reference = It::reference;

    striding_iterator(It&& _iter, int _stride) : iter(std::forward<It>(_iter)), stride(_stride) { }

    constexpr striding_iterator operator++() {
        iter += stride;
        return *this;
    }

    constexpr reference operator*() const {
        return *iter;
    }

    bool operator!=(striding_iterator other) const {
        return iter != other.iter;
    }

    difference_type operator-(const striding_iterator& other) const {
        return (iter - other.iter) / stride;
    }
};


class Forest {
    std::vector<int> trees;
    int rows;
    int cols;

    void scan_line(auto from, auto to, auto visible) const {
        int max_height = *from;
        ++from;
        while (from != to) {
            *visible = *visible || *from > max_height;
            max_height = std::max(max_height, *from);
            ++from;
            ++visible;
        }
    }

    void scan_horizontal(auto beg, auto flags_beg, int row) const {
        auto from = beg + (row * cols);
        auto to = from + (cols - 1);
        auto visible = flags_beg + (row - 1) * (cols - 2);
        scan_line(from, to, visible);
    }

    void scan_vertical(auto beg, auto flags_beg, int col) const {
        striding_iterator from { beg + col, cols };
        striding_iterator to { beg + (col + (rows - 1) * cols), cols };
        striding_iterator visible { flags_beg + (col - 1), cols - 2 };
        scan_line(from, to, visible);
    }

    void scan_dist(auto from, auto to, auto dst) const {
        int curr_height = *from;
        ++from;
        auto block = std::find_if(from, to, [curr_height](int height) {
            return curr_height <= height;
        });
        
        *dst *= 1 + int(block - from);
    }

    void dist_horizontal(auto beg, auto dst_beg, int row, int col) const {
        auto from = beg + ((row * cols) + col);
        auto to = from + (cols - col - 1);
        auto dst = dst_beg + ((row - 1) * (cols - 2) + (col - 1));
        scan_dist(from, to, dst);
    }

    void dist_vertical(auto beg, auto dst_beg, int row, int col) const {
        striding_iterator from { beg + ((row * cols) + col), cols };
        striding_iterator to { beg + ((rows - 1) * cols + col), cols };
        striding_iterator dst { dst_beg + ((row - 1) * (cols - 2) + (col - 1)), cols - 2 };
        scan_dist(from, to, dst);
    }

public:
    Forest(std::vector<int> &&heights, int columns)
        : trees(std::move(heights)), rows(trees.size() / columns), cols(columns)
    {}

    int visible_trees() const {
        std::vector<bool> flags((rows - 2) * (cols - 2));

        for (int row = 1; row < rows - 1; ++row) {
            scan_horizontal(trees.begin(), flags.begin(), row);
            scan_horizontal(trees.rbegin(), flags.rbegin(), row);
        }

        for (int col = 1; col < cols - 1; ++col) {
            scan_vertical(trees.begin(), flags.begin(), col);
            scan_vertical(trees.rbegin(), flags.rbegin(), col);
        }

        return (rows + cols) * 2 - 4 + std::reduce(flags.begin(), flags.end(), 0);
    }

    int best_scenic_score() const {
        std::vector<int> score((rows - 2) * (cols - 2), 1);

        for (int row = 1; row < rows - 1; ++row) {
            for (int col = 1; col < cols - 1; ++col) {
                dist_horizontal(trees.begin(), score.begin(), row, col);
                dist_horizontal(trees.rbegin(), score.rbegin(), row, col);
                dist_vertical(trees.begin(), score.begin(), row, col);
                dist_vertical(trees.rbegin(), score.rbegin(), row, col);
            }
        }

        return *std::max_element(score.begin(), score.end());
    }
};


int detect_columns(std::istream& in) {
    std::string line;
    std::getline(in, line);
    in.seekg(0);
    return line.length();
}

std::vector<int> read_heights(std::istream& in) {
    std::istreambuf_iterator<char> from{in}, to;
    std::vector<int> heights;

    auto stream = std::ranges::subrange(from, to)
        | std::views::filter([](char ch) { return '0' <= ch && ch <= '9'; })
        | std::views::transform([](char ch) -> int { return ch - '0'; });

    // unfortunately std::ranges::to is C++23
    // GCC 12 can't find iterator_category in range iterators, so constructing
    // vector from stream.begin()/.end() or use of std::copy fails.
    for (int height : stream) {
        heights.push_back(height);
    }

    return heights;
}


int main() {
    std::ifstream in {"day8.txt"};
    int cols = detect_columns(in);

    Forest forest(read_heights(in), cols);
    
    std::cout << "Visible (a): " << forest.visible_trees() << "\n";
    std::cout << "Best scenic score (b): " << forest.best_scenic_score() << std::endl;
    return 0;
}
