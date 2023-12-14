constexpr auto title = "--- Day 5: If You Give A Seed A Fertilizer ---";

#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

struct map_entry {
    uint64_t dst, src, span;
};

struct range {
    uint64_t strt;
    uint32_t span;
};

static constexpr auto n_maps = 7;
static vector<uint64_t> seeds;
static vector<map_entry> maps[n_maps];

static vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

static vector<uint64_t> split_numbers(string& s) {
    vector<uint64_t> result;
    stringstream ss(s);
    uint64_t n;
    while (ss >> n) result.push_back(n);
    return result;
}

static bool in_range(uint64_t src, map_entry& e) { return e.src <= src && e.src + e.span > src; }

static uint64_t find_dest(uint64_t src, int map_index) {
    for (auto& m : maps[map_index])
        if (in_range(src, m)) return m.dst + (src - m.src);
    return src;
}

static uint64_t convert(uint64_t seed) {
    for (int i = 0; i < n_maps; ++i) seed = find_dest(seed, i);
    return seed;
}

static vector<range> merge_ranges(range& rnge, int map_index) {
    vector<range> in = {rnge}, out;
    range r_out, r_in1, r_in2;
    for (auto& e : maps[map_index]) {
        vector<range> temp;
        for (int j = 0; j < in.size(); ++j) {
            range r = in[j];
            if (in_range(r.strt, e) || in_range(r.strt + r.span, e) ||
                (r.strt < e.src && r.strt + r.span >= e.src + e.span)) {
                if (r.strt < e.src && r.strt + r.span < e.src + e.span) {
                    r_out.strt = e.dst;
                    r_out.span = (r.strt + r.span) - e.src;
                    r_in1.strt = r.strt;
                    r_in1.span = e.src - r.strt;
                    if (r_in1.span) temp.push_back(r_in1);
                } else if (r.strt + r.span > e.src + e.span && r.strt >= e.src) {
                    r_out.strt = e.dst + (r.strt - e.src);
                    r_out.span = (e.src + e.span) - r.strt;
                    r_in1.strt = e.src + e.span;
                    r_in1.span = (r.strt + r.span) - (e.src + e.span);
                    if (r_in1.span) temp.push_back(r_in1);
                } else if (r.strt <= e.src && r.strt + r.span >= e.src + e.span) {
                    r_out.strt = e.dst;
                    r_out.span = e.span;
                    r_in1.strt = r.strt;
                    r_in1.span = e.src - r.strt;
                    r_in2.strt = e.src + e.span;
                    r_in2.span = (r.strt + r.span) - (e.src + e.span);
                    if (r_in1.span) temp.push_back(r_in1);
                    if (r_in2.span) temp.push_back(r_in2);
                } else {
                    r_out.strt = e.dst + (r.strt - e.src);
                    r_out.span = r.span;
                }
                out.push_back(r_out);
            } else
                temp.push_back(r);
        }
        in = temp;
    }
    if (in.size()) out.insert(out.end(), in.begin(), in.end());
    return out;
}

static uint64_t least;

static uint64_t part1() {
    least = numeric_limits<uint64_t>::max();
    for (auto& s : seeds) least = min(convert(s), least);
    return least;
}

static int part2() {
    least = numeric_limits<uint64_t>::max();
    vector<range> r;
    for (int i = 0; i < seeds.size(); i += 2) r.push_back({seeds[i], uint32_t(seeds[i + 1])});
    for (int i = 0; i < n_maps; i++) {
        vector<range> new_r;
        for (int j = 0; j < r.size(); ++j) {
            vector<range> result = merge_ranges(r[j], i);
            new_r.insert(new_r.end(), result.begin(), result.end());
        }
        r = new_r;
    }
    for (auto& e : r) least = min(e.strt, least);
    return least;
}

static const vector<string> lines = {
#include "day05.txt"
};

int main() {
    stdio_init_all();
    auto strt = time_us_32();
    string line;
    int number;
    seeds = split_numbers(split(lines[0], ':')[1]);
    int i = 0;
    for (int l = 2; l < lines.size(); l++) {
        line = lines[l];
        if (line.size() == 0) {
            l++;
            i++;
            continue;
        }
        vector<uint64_t> v = split_numbers(line);
        maps[i].push_back({v[0], v[1], v[2]});
    }
    cout << title << endl
         << "Part 1  - " << part1() << endl
         << "Part 2  - " << part2() << endl
         << "Elapsed - " << (time_us_32() - strt) / 1000.0 << " ms." << endl;
}
