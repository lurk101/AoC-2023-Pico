#define title "--- Day 9: Mirage Maintenance ---"
#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<unsigned> split_numbers(const string& s) {
    vector<unsigned> result;
    stringstream ss(s);
    int n;
    while (ss >> n) result.push_back(n);
    return result;
}

static vector<vector<unsigned>> vv;

static bool all_zeros(vector<int>& v) {
    for (auto val : v)
        if (val) return false;
    return true;
}

static unsigned part(int part_n) {
    unsigned sum = 0;
    for (auto& v : vv) {
        vector<vector<int>> deltas = {{}};
        for (int i = 0; i < v.size() - 1; i++) deltas.back().push_back(v[i + 1] - v[i]);
        while (!all_zeros(deltas.back())) {
            deltas.push_back({});
            for (int i = 0; i < deltas[deltas.size() - 2].size() - 1; i++)
                deltas.back().push_back(deltas[deltas.size() - 2][i + 1] -
                                        deltas[deltas.size() - 2][i]);
        }
        unsigned sum2 = 0;
        if (part_n == 1) {
            for (auto& v : deltas) sum2 += v.back();
            sum += v.back() + sum2;
        } else {
            for (int i = deltas.size() - 1; i >= 0; i--) sum2 = deltas[i].front() - sum2;
            sum += v.front() - sum2;
        }
    }
    return sum;
}

static const vector<string> lines = {
#include "day09.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    string line;
    for (auto& line : lines) vv.push_back(split_numbers(line));
    cout << title << endl
         << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(2) << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
