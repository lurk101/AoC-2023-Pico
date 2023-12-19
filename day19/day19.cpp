constexpr auto title = "--- Day 19: Aplenty ---";

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include <pico/stdlib.h>

using namespace std;
using namespace chrono;

constexpr array<char, 4> rule_index{'x', 'm', 'a', 's'};

struct rule_t {
    int index, value;
    bool less;
    string target;
    bool eval(const array<int, 4>& p) const {
        return less ? p[index] < value : p[index] > value;
    }
};

struct flow_t {
    vector<rule_t> rules;
    string all_false;
    void process_rule(const string s) {
        stringstream ss(s);
        string line;
        while (getline(ss, line, ',')) {
            auto p = line.find(':');
            if (p == string::npos) {
                all_false = line;
                break;
            }
            rule_t r;
            r.target = line.substr(p + 1);
            line = line.substr(0, p);
            r.index =
                distance(rule_index.cbegin(), find(rule_index.begin(), rule_index.end(), line[0]));
            r.less = line[1] == '<';
            r.value = stoi(line.substr(2));
            rules.push_back(r);
        }
    }
    const string& next(const array<int, 4>& p) const {
        for (const auto& r : rules)
            if (r.eval(p)) return r.target;
        return all_false;
    }
};

uint64_t combos(map<string, flow_t>& flows, const string& rule_name,
                array<pair<int64_t, int64_t>, 4> vr) {
    if (rule_name == "R") return 0;
    if (rule_name == "A")
        return accumulate(vr.begin(), vr.end(), 1ll,
                          [](int64_t a, const pair<int64_t, int64_t>& r) {
                              return a * (r.second - r.first + 1);
                          });
    int64_t result = 0;
    const flow_t& w = flows.find(rule_name)->second;
    for (const rule_t& r : w.rules) {
        array<pair<int64_t, int64_t>, 4> split = vr;
        if (r.less) {
            split[r.index].second = r.value - 1;
            vr[r.index].first = r.value;
            result += combos(flows, r.target, split);
        } else {
            split[r.index].first = r.value + 1;
            vr[r.index].second = r.value;
            result += combos(flows, r.target, split);
        }
    }
    return result + combos(flows, w.all_false, vr);
}

static const vector<string> lines = {
#include "day19.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    int64_t part1 = 0;
    string line;
    map<string, flow_t> flows;
    int i = 0;
    for (;;) {
        line = lines[i];
        if (line.empty()) break;
        flow_t w;
        auto p = line.find('{');
        string name = line.substr(0, p);
        w.process_rule(line.substr(p + 1, line.size() - p - 2));
        flows[name] = w;
        i++;
    }
    for (i++; i < lines.size(); i++) {
        line = lines[i];
        line = line.substr(1, line.size() - 2);
        stringstream ss(line);
        array<int, 4> p = {0, 0, 0, 0};
        for (int& i : p) {
            string s;
            getline(ss, s, ',');
            i = stoi(s.substr(2));
        }
        string name = "in";
        while (name != "A" && name != "R") name = flows.find(name)->second.next(p);
        if (name == "A")
            for (int i : p) part1 += i;
    }
    constexpr array<pair<int64_t, int64_t>, 4> initial = {make_pair(1, 4000), make_pair(1, 4000),
                                                          make_pair(1, 4000), make_pair(1, 4000)};
    int64_t part2 = combos(flows, "in", initial);
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
