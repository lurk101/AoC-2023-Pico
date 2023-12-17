constexpr auto title = "--- Day 16: The Floor Will Be Leftava ---";

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

enum dir { Right, Down, Left, Up };

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
            switch (grid[beam.y][beam.x]) {  // 0 - Right,
            case '.':
                if (beam.d == Right && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, beam.d});
                else if (beam.d == Left && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, beam.d});
                else if (beam.d == Down && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, beam.d});
                else if (beam.d == Up && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, beam.d});
                break;
            case '/':
                if (beam.d == Up && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, Right});
                else if (beam.d == Down && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, Left});
                else if (beam.d == Left && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, Down});
                else if (beam.d == Right && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, Up});
                break;
            case '\\':
                if (beam.d == Down && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, Right});
                else if (beam.d == Up && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, Left});
                else if (beam.d == Right && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, Down});
                else if (beam.d == Left && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, Up});
                break;
            case '-':
                if (beam.d == Right && beam.x < grid[0].size() - 1)
                    beams.push_back({beam.x + 1, beam.y, Right});
                else if (beam.d == Left && beam.x > 0)
                    beams.push_back({beam.x - 1, beam.y, Left});
                else if (beam.d == Down || beam.d == Up) {
                    if (beam.x < grid[0].size() - 1) beams.push_back({beam.x + 1, beam.y, Right});
                    if (beam.x > 0) beams.push_back({beam.x - 1, beam.y, Left});
                }
                break;
            case '|':
                if (beam.d == Down && beam.y < grid.size() - 1)
                    beams.push_back({beam.x, beam.y + 1, Down});
                else if (beam.d == Up && beam.y > 0)
                    beams.push_back({beam.x, beam.y - 1, Up});
                else if (beam.d == Right || beam.d == Left) {
                    if (beam.y < grid.size() - 1) beams.push_back({beam.x, beam.y + 1, Down});
                    if (beam.y > 0) beams.push_back({beam.x, beam.y - 1, Up});
                }
                break;
            }
        }
    }
    int count = 0;
    for (auto& row : visited)
        for (auto c : row)
            if (c) count++;
    return count;
}

static uint32_t sides[4];

void t(int i) {
    uint32_t part2 = 0;
    switch (i) {
    case 0:
        for (int i = 0; i < grid.size(); i++) {
            beams[0].push_back({0, i, Right});
            part2 = max(energy(beams[0]), part2);
        }
        break;
    case 1:
        for (int i = 0; i < grid.size(); i++) {
            beams[1].push_back({int(grid.size() - 1), i, Left});
            part2 = max(energy(beams[1]), part2);
        }
        break;
    case 2:
        for (int i = 0; i < grid[0].size(); i++) {
            beams[2].push_back({i, 0, Down});
            part2 = max(energy(beams[2]), part2);
        }
        break;
    case 3:
        for (int i = 0; i < grid[0].size(); i++) {
            beams[3].push_back({i, int(grid[0].size() - 1), Up});
            part2 = max(energy(beams[3]), part2);
        }
        break;
    }
    sides[i] = part2;
}

int main() {
    stdio_init_all();
    auto start = time_us_32();
    beams[0].push_back({0, 0, Right});
    uint32_t part1 = energy(beams[0]), part2 = 0;
    thread* threads[4];
    for (int i = 0; i < 4; i++) t(i);
    for (int i = 0; i < 4; i++) part2 = max(part2, sides[i]);
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
