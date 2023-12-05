#define title "--- Day 1: Trebuchet?! ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "pico/stdlib.h"


using namespace std;
using namespace chrono;

static int part1(string s) {
    int first(-1), last;
    for (int i = 0; i < s.size(); ++i) {
        if (isdigit(s[i])) {
            if (first < 0)
                first = s[i] - '0';
            last = s[i] - '0';
        }
    }
    if (first < 0)
        return 0;
    return first * 10 + last;
}

static int part2(string s) {
    struct number {
        string txt;
        int val;
    };
    const number numbers[] = {{"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
                              {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"1", 1},
                              {"2", 2},   {"3", 3},     {"4", 4},     {"5", 5},    {"6", 6},
                              {"7", 7},   {"8", 8},     {"9", 9}};
    int first(-1), last;
    for (int i = 0; i < s.size(); ++i) {
        bool found(false);
        for (auto& num : numbers) {
            if (s.substr(i, num.txt.length()) == num.txt) {
                first = num.val;
                found = true;
                break;
            }
        }
        if (found) break;
    }
    if (first < 0) return 0;
    for (int i = s.size() - 1; i >= 0; --i) {
        bool found(false);
        for (auto& num : numbers) {
            if (s.substr(i, num.txt.length()) == num.txt) {
                last = num.val;
                found = true;
                break;
            }
        }
        if (found) break;
    }
    return first * 10 + last;
}

static const vector<string> lines = {
#include "day01.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    string line;
    unsigned int part1_sum(0), part2_sum(0);
    for (auto& line : lines) {
        part1_sum += part1(line);
        part2_sum += part2(line);
    }
    cout << title << endl
         << "Part 1  - " << part1_sum << endl
         << "Part 2  - " << part2_sum << endl
         << "Elapsed - "
         << (time_us_32() - start) / 1000.0
         << " ms." << endl;
}
