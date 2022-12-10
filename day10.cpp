#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Cpu;

class Commands {
    std::istream& in;

public:
    Commands(std::istream& _in) : in(_in) { }

    void next(Cpu& cpu);
};

class Cpu {
    int x = 1;
    int prev_x = 1;
    int current_cycle = 1;
    Commands& commands;

public:
    Cpu(Commands& stream) : commands(stream) { }

    void noop() {
        ++current_cycle;
    }
    
    void addx(int value) {
        current_cycle += 2;
        prev_x = x;
        x += value;
    }

    int registerAt(int cycle) {
        while (current_cycle < cycle) {
            commands.next(*this);
        }
        return (cycle < current_cycle) ? prev_x : x;
    }
};

void Commands::next(Cpu& cpu) {
    std::string line;
    std::getline(in, line);
    if (line == "noop") {
        cpu.noop();
    } else if (line.starts_with("addx ")) {
        cpu.addx(std::stoi(line.substr(5)));
    }
}

constexpr bool is_lit(int reg, int col) {
    return -1 <= (col - reg) && (col - reg) <= 1;
}

int main() {
    std::ifstream in("day10.txt");
    Commands commands(in);
    Cpu cpu(commands);

    int strength = 0;
    for (int row = 0; row < 6; ++row) {
        for (int col = 1; col <= 40; ++col) {
            int cycle = row * 40 + col;
            int reg = cpu.registerAt(cycle);
            if (col == 20) {
                strength += reg * (row * 40 + col);
            }

            std::cout << (is_lit(reg, col - 1) ? '#' : '.');
        }
        std::cout << "\n";
    }

    std::cout << "Strength (a): " << strength << "\n";
    return 0;
}
