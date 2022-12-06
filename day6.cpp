#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>


class Window {
    std::vector<char> buffer;
    int to_skip = 0;
    int offset = 0;

public:
    Window(int length) : buffer(length) {}

    int get_offset() const {
        return offset;
    }

    bool move_unique(char ch) {
        auto found = std::find(buffer.rbegin(), buffer.rend(), ch);

        to_skip -= to_skip > 0;
        to_skip = std::max(int(buffer.rend() - found), to_skip);

        buffer.erase(buffer.begin());
        buffer.push_back(ch);
        
        offset += 1;
        return found == buffer.rend() && to_skip == 0;
    }
};


int main() {
    std::ifstream in{"day6.txt"};
    std::istreambuf_iterator<char> from{in}, to;

    Window windowPacket(3);
    Window windowMessage(13);
    std::for_each_n(from, 3, [&windowPacket, &windowMessage](char ch) {
        windowPacket.move_unique(ch);
        windowMessage.move_unique(ch);
    });

    bool packetFound = false;
    bool messageFound = false;
    while (from != to && (!packetFound || !messageFound)) {
        if (!packetFound) {
            packetFound = windowPacket.move_unique(*from);
        }
        if (!messageFound) {
            messageFound = windowMessage.move_unique(*from);
        }
        from++;
    }

    std::cout << "Packet (a): " << windowPacket.get_offset() << "\n";
    std::cout << "Message (b): " << windowMessage.get_offset() << std::endl;
    return 0;
}
