#define title "--- Day 3: Gear Ratios ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "pico/stdlib.h"

using namespace std;
using namespace chrono;

struct part {
    int val, y, x_start, x_end;
};

struct gear {
    int x, y, count, product;
};

static vector<string> engine;
static vector<part> parts;
static vector<gear> gears;

static void update_gear(int x, int y, int v) {
    for (auto& g : gears)
        if (g.x == x && g.y == y) {
            g.count++;
            g.product *= v;
            break;
        }
}

static const vector<string> lines = {
#include "day03.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    ifstream f("day03.txt");
    string line;
    for (auto& line : lines) engine.push_back(line);
    const int x_max = engine[0].size() - 1;
    const int y_max = engine.size() - 1;
    for (int y = 0; y <= y_max; y++) {
        line = engine[y];
        for (int x = 0; x <= x_max; x++) {
            if (line[x] == '*') {
                gear g({x, y, 0, 1});
                gears.push_back(g);
            }
            if (!isdigit(line[x])) continue;
            part p({0, y, x, 0});
            int x2;
            for (x2 = x; x2 <= x_max; x2++) {
                if (!isdigit(line[x2])) break;
                p.val = p.val * 10 + line[x2] - '0';
            }
            x = x2 - 1;
            p.x_end = x;
            parts.push_back(p);
        }
    }
    int part1 = 0;
    for (auto& p : parts) {
        bool good_part = false;
        if (p.x_start > 0 && engine[p.y][p.x_start - 1] != '.') {
            good_part = true;
            if (engine[p.y][p.x_start - 1] == '*') update_gear(p.x_start - 1, p.y, p.val);
        }
        if (p.x_end < x_max && engine[p.y][p.x_end + 1] != '.') {
            good_part = true;
            if (engine[p.y][p.x_end + 1] == '*') update_gear(p.x_end + 1, p.y, p.val);
        }
        int start = max(0, p.x_start - 1);
        int end = min(x_max, p.x_end + 1);
        if (p.y > 0)
            for (int x = start; x <= end; x++)
                if (engine[p.y - 1][x] != '.') {
                    good_part = true;
                    if (engine[p.y - 1][x] == '*') update_gear(x, p.y - 1, p.val);
                }
        if (p.y < y_max)
            for (int x = start; x <= end; x++)
                if (engine[p.y + 1][x] != '.') {
                    good_part = true;
                    if (engine[p.y + 1][x] == '*') update_gear(x, p.y + 1, p.val);
                }
        if (good_part) part1 += p.val;
    }
    int part2 = 0;
    for (auto& g : gears)
        if (g.count == 2) part2 += g.product;
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - "
         << (time_us_32() - start) / 1000.0
         << " ms." << endl;
}
