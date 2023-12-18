constexpr auto title = "--- Day 18: Lavaduct Lagoon ---";

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <pico/stdlib.h>

using namespace std;
using namespace chrono;

static vector<pair<int64_t, int64_t>> vertices;
static const vector<string> lines = {
#include "day18.txt"
};

static vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

static int64_t solve(int part) {
    vertices.clear();
    int64_t result = 0;
    int64_t max = 0, min = numeric_limits<int64_t>::max();
    int dir, x = 0, y = 0, steps, i = 0;
    for (auto& line : lines) {
        auto v = split(line, ' ');
        switch (v[0][0]) {
        case 'U':
            dir = 0;
            break;
        case 'R':
            dir = 1;
            break;
        case 'D':
            dir = 2;
            break;
        case 'L':
            dir = 3;
            break;
        }
        steps = strtoll(v[1].c_str(), NULL, 10);
        if (part == 2) {
            dir = strtoll(v[2].substr(7, 1).c_str(), NULL, 10) + 1;
            dir %= 4;
            steps = strtoll(v[2].substr(2, 5).c_str(), NULL, 16);
        }
        switch (dir) {
        case 0:
            x -= steps;
            break;
        case 1:
            y += steps;
            break;
        case 2:
            x += steps;
            break;
        case 3:
            y -= steps;
            break;
        }
        vertices.push_back(make_pair(x, y));
        i++;
        if (x > max) max = x;
        if (x < min) min = x;
    }
    vertices.push_back(make_pair(vertices[0].first, vertices[0].second));
    for(; i > 0; i--)
    {
        int64_t a = vertices[i].second * vertices[i - 1].first;
        int64_t b = vertices[i].first * vertices[i - 1].second;
        int64_t c = vertices[i].first - vertices[i - 1].first;
        int64_t d = vertices[i].second - vertices[i - 1].second;
        result+=(labs(c)+labs(d));
        result+=(b-a);
    }
    return result / 2 + 1;
}

int main() {
    stdio_init_all();
    auto start = time_us_32();
    cout << title << endl
         << "Part 1  - " << solve(1) << endl
         << "Part 2  - " << solve(2) << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
