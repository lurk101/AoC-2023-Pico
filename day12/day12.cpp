constexpr auto title = "--- Day 12: Hot Springs ---";
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pico/stdlib.h"

using namespace std;
using namespace chrono;

static vector<string> split(const string& s, char delim) {
    vector<string> count;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) count.push_back(item);
    return count;
}

static vector<uint32_t> split_numbers(string& s) {
    vector<uint32_t> count;
    stringstream ss(s);
    int n;
    char c;
    while (ss >> n) {
        count.push_back(n);
        ss >> c;
    }
    return count;
}

static uint64_t solve(string s, vector<uint32_t> c) {
    struct key {
        uint32_t k[4];
        bool operator<(key a) const {
            for (int i = 0; i < 4; i++)
                if (k[i] != a.k[i]) return k[i] < a.k[i];
            return false;
        }
    };
    map<key, uint64_t> states, nstates;
    uint64_t count = 0;
    states[{0, 0, 0, 0}] = 1;
    while (states.size()) {
        uint32_t si, ci, cc, dot;
        uint64_t num;
        for (auto& cs : states) {
            num = cs.second;
            si = cs.first.k[0];
            ci = cs.first.k[1];
            cc = cs.first.k[2];
            dot = cs.first.k[3];
            if (si == s.size()) {
                if (ci == c.size()) count += num;
                continue;
            }
            if ((s[si] == '#' || s[si] == '?') && ci < c.size() && dot == 0) {
                if (s[si] == '?' && cc == 0) nstates[{si + 1, ci, cc, dot}] += num;
                cc++;
                if (cc == c[ci]) {
                    ci++;
                    cc = 0;
                    dot = 1;
                }
                nstates[{si + 1, ci, cc, dot}] += num;
            } else if ((s[si] == '.' || s[si] == '?') && cc == 0) {
                dot = 0;
                nstates[{si + 1, ci, cc, dot}] += num;
            }
        }
        states = nstates;
        nstates.clear();
    }
    return count;
}

static const vector<string> lines = {
#include "day12.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    string line;
    uint64_t p1 = 0, p2 = 0;
    for (auto& l : lines) {
        auto ll = split(l, ' ');
        string s = ll[0];
        auto v = split_numbers(ll[1]);
        p1 += solve(s, v);
        string p2s = s;
        vector<uint32_t> p2v = v;
        for (uint32_t j = 0; j < 4; j++) {
            p2s += '?';
            p2s += s;
            for (int j = 0; j < v.size(); j++) p2v.push_back(v[j]);
        }
        p2 += solve(p2s, p2v);
    }
    cout << title << endl
         << "Part 1  - " << p1 << endl
         << "Part 2  - " << p2 << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
