#define title "--- Day 4: Scratchcards ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "pico/stdlib.h"

using namespace std;
using namespace chrono;

static vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

static vector<int> split_numbers(string& s) {
    vector<int> result;
    stringstream ss(s);
    int n;
    while (ss >> n) result.push_back(n);
    return result;
}

struct card {
    int instances = 1;
    int match = 0;
    vector<int> winning, have;
};

vector<card> cards;

static const vector<string> lines = {
#include "day04.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    ifstream f("day04.txt");
    string line;
    for (auto& line : lines) {
        vector<string> s = split(split(line, ':')[1], '|');
        card c;
        c.winning = split_numbers(s[0]);
        c.have = split_numbers(s[1]);
        cards.push_back(c);
    }
    int part1 = 0;
    for (auto& c : cards) {
        for (auto h : c.have)
            for (auto w : c.winning)
                if (h == w) {
                    c.match++;
                    break;
                }
        part1 += 1 << (c.match - 1);
    }
    int part2 = 0;
    for (int i = 0; i < cards.size(); i++) {
        for (int j = i + 1; j < min(i + 1 + cards[i].match, int(cards.size())); j++)
            cards[j].instances += cards[i].instances;
        part2 += cards[i].instances;
    }
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - "
         << (time_us_32() - start) / 1000.0
         << " ms." << endl;
}
