constexpr auto title = "--- Day 8: Haunted Wasteland ---";
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <pico/stdlib.h>

using namespace std;
using namespace chrono;

struct direction {
    string left, right;
};

map<string, direction> wasteland;
string directions;

static unsigned part1(void) {
    unsigned count = 0;
    string loc = "AAA";
    unsigned d_index = 0;
    while (loc != "ZZZ") {
        count++;
        loc = directions[d_index] == 'L' ? wasteland[loc].left : wasteland[loc].right;
        d_index = (d_index == directions.size() - 1) ? 0 : d_index + 1;
    }
    return count;
}

static long long part2(void) {
    vector<string> locs;
    for (auto& w : wasteland)
        if (w.first[2] == 'A') locs.push_back(w.first);
    vector<long long> counts;
    for (auto loc : locs) {
        unsigned count = 0;
        unsigned d_index = 0;
        while (loc[2] != 'Z') {
            count++;
            loc = directions[d_index] == 'L' ? wasteland[loc].left : wasteland[loc].right;
            d_index = (d_index == directions.size() - 1) ? 0 : d_index + 1;
        }
        counts.push_back(count);
    }
    long long l = 1;
    for (auto c : counts) l = lcm(l, c);
    return l;
}

static const vector<string> lines = {
#include "day08.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    string line;
    directions = line[0];
    for (int i = 2; i < lines.size(); i++)
        wasteland[lines[i].substr(0, 3)] = {lines[i].substr(7, 3), lines[i].substr(12, 3)};
    cout << title << endl
         << "Part 1  - " << part1() << endl
         << "Part 2  - " << part2() << endl
         << "Elapsed - "
         << (time_us_32() - start) / 1000.0
         << " ms." << endl;
}
