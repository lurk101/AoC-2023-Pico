constexpr auto title = "--- Day 16: The Floor Will Be Lava ---";

#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

enum dir { R, D, L, U };

struct beam_t {
    int x, y;
    dir d;
};

static list<beam_t> beams[4];
static const vector<string> grid = {
#include "day16.txt"
};

static uint32_t energy(list<beam_t>& beams) {
    const vector<uint8_t> visited_row(grid[0].size(), 0);
    vector<vector<uint8_t>> visited(grid.size(), visited_row);
    while (beams.size()) {
        beam_t beam = beams.front();
        beams.pop_front();
        if (!(visited[beam.y][beam.x] & (1 << beam.d))) {
            visited[beam.y][beam.x] |= 1 << beam.d;
            switch (grid[beam.y][beam.x]) {  // 0 - R,
            case '.':
                if (beam.d == R && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, beam.d});
                else if (beam.d == L && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, beam.d});
                else if (beam.d == D && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, beam.d});
                else if (beam.d == U && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, beam.d});
                break;
            case '/':
                if (beam.d == U && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, R});
                else if (beam.d == D && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, L});
                else if (beam.d == L && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, D});
                else if (beam.d == R && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, U});
                break;
            case '\\':
                if (beam.d == D && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, R});
                else if (beam.d == U && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, L});
                else if (beam.d == R && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, D});
                else if (beam.d == L && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, U});
                break;
            case '-':
                if (beam.d == R && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, R});
                else if (beam.d == L && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, L});
                else if (beam.d == D || beam.d == U) {
                    if (beam.x < grid[0].size() - 1) beams.push_back({beam.x + 1, beam.y, R});
                    if (beam.x > 0) beams.push_back({beam.x - 1, beam.y, L});
                }
                break;
            case '|':
                if (beam.d == D && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, D});
                else if (beam.d == U && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, U});
                else if (beam.d == R || beam.d == L) {
                    if (beam.y < grid.size() - 1) beams.push_back({beam.x, beam.y + 1, D});
                    if (beam.y > 0) beams.push_back({beam.x, beam.y - 1, U});
                }
                break;
            }
        }
    }
    int count = 0;
    for (int y = 0; y < grid.size(); y++)
        for (int x = 0; x < grid[0].size(); x++)
            if (visited[y][x]) count++;
    return count;
}

static uint32_t sides[4];

void t(int i) {
    uint32_t part2 = 0;
    switch (i) {
    case 0:
        for (int i = 0; i < grid.size(); i++) {
            beams[0].push_back({0, i, R});
            uint32_t p2 = energy(beams[0]);
            part2 = max(p2, part2);
        }
        break;
    case 1:
        for (int i = 0; i < grid.size(); i++) {
            beams[1].push_back({int(grid.size() - 1), i, L});
            uint32_t p2 = energy(beams[1]);
            part2 = max(p2, part2);
        }
        break;
    case 2:
        for (int i = 0; i < grid[0].size(); i++) {
            beams[2].push_back({i, 0, D});
            uint32_t p2 = energy(beams[2]);
            part2 = max(p2, part2);
        }
        break;
    case 3:
        for (int i = 0; i < grid[0].size(); i++) {
            beams[3].push_back({i, int(grid[0].size() - 1), U});
            uint32_t p2 = energy(beams[3]);
            part2 = max(p2, part2);
        }
        break;
    }
    sides[i] = part2;
}

int main() {
    stdio_init_all();
    auto start = time_us_32();
    beams[0].push_back({0, 0, R});
    uint32_t part1 = energy(beams[0]), part2 = 0;
    thread* threads[4];
    for (int i = 0; i < 4; i++) t(i);
    for (int i = 0; i < 4; i++) part2 = max(part2, sides[i]);
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
