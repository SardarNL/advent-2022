#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <utility>
#include <string>
#include <regex>


std::regex NUMBERS("\\d+");


class Monkey;

class Playground {
    std::vector<Monkey> monkeys;
    long common_divisor;

public:
    Playground(const std::vector<Monkey> &_monkeys);

    void rounds(int max);

    void roundsDiv3(int max);

    void to_monkey(long worry, int monkey);

    long monkey_business(int max);
};

class Monkey {
    std::vector<long> items;
    std::function<long(long)> operation;
    long test_divisible;
    int monkey_true;
    int monkey_false;

    int inspected = 0;
public:
    void play(Playground &playground, auto limiter) {
        inspected += items.size();
        for (long worry : items) {
            long new_worry = limiter(operation(worry));
            playground.to_monkey(new_worry, (new_worry % test_divisible) == 0 ? monkey_true : monkey_false);
        }
        items.clear();
    }

    friend class Playground;
    friend std::istream& operator>>(std::istream &in, Monkey &monkey);
};


Playground::Playground(const std::vector<Monkey> &_monkeys) : monkeys(_monkeys) {
    common_divisor = std::transform_reduce(monkeys.begin(), monkeys.end(), 1l, std::multiplies<>(), [](const Monkey &monkey) {
        return monkey.test_divisible;
    });
}

void Playground::rounds(int max) {
    int cm = common_divisor;
    for (int i = 0; i < max; ++i) {
        for (auto &monkey : monkeys) {
            monkey.play(*this, [cm](long worry) {
                return worry % cm;
            });
        }
    }
}

void Playground::roundsDiv3(int max) {
    for (int i = 0; i < max; ++i) {
        for (auto &monkey : monkeys) {
            monkey.play(*this, [](long worry) {
                return worry / 3;
            });
        }
    }
}

void Playground::to_monkey(long worry, int monkey) {
    monkeys[monkey].items.emplace_back(worry);
}

long Playground::monkey_business(int max) {
    std::vector<long> inspected;
    std::transform(monkeys.begin(), monkeys.end(), std::back_inserter(inspected), [](auto const& monkey) {
        return monkey.inspected;
    });
    std::sort(inspected.rbegin(), inspected.rend());
    return std::accumulate(inspected.begin(), inspected.begin() + max, 1l, std::multiplies<>());
}

std::istream& operator>>(std::istream &in, Monkey &monkey) {
    std::string line;
    std::getline(in, line); // Monkey 0:
    if (line.empty()) {
        return in;
    }

    std::getline(in, line); // Starting items: 1, 2...
    std::sregex_iterator from(line.begin(), line.end(), NUMBERS), to;

    monkey.items.clear();
    std::transform(from, to, std::back_inserter(monkey.items), [](auto const &match) {
        return std::stol(match.str());
    });

    std::getline(in, line); // Operation: new = old + [arg]
    line.erase(0, 19);
    if (line == "old * old") {
        monkey.operation = [](long worry) { return worry * worry; };
    } else {
        int arg = std::stoi(line.substr(6));
        if (line.starts_with("old + ")) {
            monkey.operation = [arg](long worry) { return worry + arg; };
        } else {
            monkey.operation = [arg](long worry) { return worry * arg; };
        }
    }

    std::getline(in, line); // Test: divisible by 123
    monkey.test_divisible = std::stol(line.substr(21));

    std::getline(in, line); // If true: throw to monkey 1
    monkey.monkey_true = std::stoi(line.substr(29));

    std::getline(in, line); // If false: throw to monkey 3
    monkey.monkey_false = std::stoi(line.substr(30));

    in >> std::ws;
    return in;
}

int main() {
    std::ifstream in("day11.txt");
    std::istream_iterator<Monkey> from(in), to;
    std::vector<Monkey> monkeys(from, to);

    Playground playgroundDiv3(monkeys);
    Playground playground(monkeys);

    playgroundDiv3.roundsDiv3(20);
    playground.rounds(10000);

    std::cout << "Monkey business 20 div 3 (a): " << playgroundDiv3.monkey_business(2) << "\n";
    std::cout << "Monkey business 10.000 (b): " << playground.monkey_business(2) << "\n";
    return 0;
}
