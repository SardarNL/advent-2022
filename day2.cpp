#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>



std::unordered_map<std::string, int> scores_choice {
    { "A X", 1 + 3 },
    { "A Y", 2 + 6 },
    { "A Z", 3 + 0 },
    { "B X", 1 + 0 },
    { "B Y", 2 + 3 },
    { "B Z", 3 + 6 },
    { "C X", 1 + 6 },
    { "C Y", 2 + 0 },
    { "C Z", 3 + 3 },
};

std::unordered_map<std::string, int> scores_goal {
    { "A X", 3 + 0 },
    { "A Y", 1 + 3 },
    { "A Z", 2 + 6 },
    { "B X", 1 + 0 },
    { "B Y", 2 + 3 },
    { "B Z", 3 + 6 },
    { "C X", 2 + 0 },
    { "C Y", 3 + 3 },
    { "C Z", 1 + 6 },
};



int main() {
    std::ifstream in {"day2.txt"};
    std::string line;
    int score_choice = 0;
    int score_goal = 0;

    while(std::getline(in, line)) {
        score_choice += scores_choice.at(line);
        score_goal += scores_goal.at(line);
    }

    std::cout << "Choice (a): " << score_choice << "\n";
    std::cout << "Goal (b): " << score_goal << std::endl;
    return 0;
}
