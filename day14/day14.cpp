constexpr auto title = "--- Day 14: Parabolic Reflector Dish ---";

#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "pico/stdlib.h"

using namespace std;
using namespace chrono;

static void roll(vector<string>& g) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (uint32_t x = 0; x < g[0].size(); x++)
            for (uint32_t y = 0; y < g.size() - 1; y++) {
                auto& curr = g[y][x];
                auto& above = g[y + 1][x];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
                    changed = true;
                }
            }
    }
    changed = true;
    while (changed) {
        changed = false;
        for (uint32_t y = 0; y < g.size(); y++)
            for (uint32_t x = 0; x < g[0].size() - 1; x++) {
                auto& curr = g[y][x];
                auto& above = g[y][x + 1];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
                    changed = true;
                }
            }
    }
    changed = true;
    while (changed) {
        changed = false;
        for (uint32_t x = 0; x < g[0].size(); x++)
            for (uint32_t y = g.size() - 1; y != 0; y--) {
                auto& curr = g[y][x];
                auto& above = g[y - 1][x];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
                    changed = true;
                }
            }
    }
    changed = true;
    while (changed) {
        changed = false;
        for (uint32_t y = 0; y < g.size(); y++)
            for (uint32_t x = g[0].size() - 1; x != 0; x--) {
                auto& curr = g[y][x];
                auto& above = g[y][x - 1];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
                    changed = true;
                }
            }
    }
}

static size_t join(const vector<string>& g) {
    const hash<string> hasher;
    string r;
    for (auto& s : g) r += s;
    return hasher(r);
}

static uint32_t total_load(const vector<string>& g) {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < g.size(); i++)
        for (uint32_t x = 0; x < g[0].size(); x++)
            if (g[i][x] == 'O') sum += g.size() - i;
    return sum;
}

static uint32_t part2(vector<string> g) {
    unordered_map<size_t, uint32_t> grids;
    unordered_map<uint32_t, uint32_t> loads;
    uint32_t i = 0, cycle_start = 0, cycle_end = 0;
    constexpr uint32_t target = 1000000000;
    while (true) {
        roll(g);
        i++;
        const auto key = join(g);
        if (grids.contains(key)) {
            cycle_start = grids.at(key);
            cycle_end = i;
            break;
        } else {
            grids[key] = i;
            loads[i] = total_load(g);
        }
    }
    const auto period = cycle_end - cycle_start;
    const auto cycle = target - cycle_start;
    const auto same = cycle_start + cycle % period;
    return loads[same];
}

static uint32_t part1(vector<string> g) {
    uint32_t i = 1;
    bool changed = true;
    while (changed) {
        changed = false;
        for (uint32_t x = 0; x < g[0].size(); x++)
            for (uint32_t y = 0; y < g.size() - 1; y++) {
                auto& curr = g[y][x];
                auto& above = g[y + 1][x];
                if (curr == '.' && above == 'O') {
                    swap(curr, above);
                    changed = true;
                }
            }
        i++;
    }
    return total_load(g);
}

static const vector<string> lines = {
#include "day14.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    string line;
    vector<string> grid;
    for (auto& line : lines) grid.push_back(line);
    cout << title << endl
         << "Part 1  - " << part1(grid) << endl
         << "Part 2  - " << part2(grid) << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
