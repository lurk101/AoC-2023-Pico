constexpr auto title = "--- Day 11: Cosmic Expansion ---";
#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static set<int> empty_columns;

static const vector<string> lines = {
#include "day11.txt"
};

static bool empty_row(string row) {
    for (char c : row)
        if (c != '.') return false;
    return true;
}

static bool empty_col(const vector<string>& lines, int col) {
    for (const string& row : lines)
        if (row[col] != '.') return false;
    return true;
}

static int distance(pair<int, int>& a, pair<int, int>& b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

int64_t part(uint32_t delta) {
    vector<pair<int, int>> galaxy;
    uint32_t y_inc = 0;
    for (int y = 0; y < lines.size(); y++) {
        if (empty_row(lines[y])) y_inc += delta;
        int x_inc = 0;
        for (int x = 0; x < lines[0].size(); x++) {
            if (empty_columns.find(x) != empty_columns.end()) x_inc += delta;
            if (lines[y][x] == '#') galaxy.push_back(make_pair(y + y_inc, x + x_inc));
        }
    }
    int64_t sum = 0;
    for (int i = 0; i < galaxy.size() - 1; i++)
        for (int j = i + 1; j < galaxy.size(); j++) sum += distance(galaxy[i], galaxy[j]);
    return sum;
}

int main() {
    stdio_init_all();
    auto start = time_us_32();
    for (int x = 0; x < lines[0].size(); x++)
        if (empty_col(lines, x)) empty_columns.insert(x);
    cout << title << endl
         << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(999999) << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
