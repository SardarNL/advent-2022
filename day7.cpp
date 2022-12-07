#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>



class Engine {
    std::vector<int> sizes;
    std::vector<int> offset_stack;

public:
    void push_directory() {
        sizes.push_back(0);
        offset_stack.push_back(sizes.size() - 1);
    }

    void pop_directory() {
        offset_stack.pop_back();
    }

    void file_size(int size) {
        for (int offset : offset_stack) {
            sizes[offset] += size;
        }
    }

    int total_directory_sizes(int max_size) const {
        int total = 0;
        for (int size : sizes) {
            if (size <= max_size) {
                total += size;
            }
        }
        return total;
    }

    int smallest_to_remove(int max_device, int needed_size) {
        int to_free = needed_size - (max_device - sizes[0]);

        std::sort(sizes.begin(), sizes.end());
        for (int size : sizes) {
            if (size >= to_free) {
                return size;
            }
        }

        return sizes.front();
    }
};


int main() {
    std::ifstream in{"day7.txt"};
    Engine engine;

    std::string line;
    while (std::getline(in, line)) {
        if (line == "$ cd ..") {
            engine.pop_directory();
        } else if (line.starts_with("$ cd ")) {
            engine.push_directory();
        } else if (!line.empty() && !line.starts_with("$ ls") && !line.starts_with("dir")) {
            engine.file_size(std::stoi(line));
        }
    }

    std::cout << "Size < 100000 (a): " << engine.total_directory_sizes(100'000) << "\n";
    std::cout << "To remove (b): " << engine.smallest_to_remove(70'000'000, 30'000'000) << std::endl;
    return 0;
}
