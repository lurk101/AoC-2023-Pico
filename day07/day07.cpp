constexpr auto title = "--- Day 7: Camel Cards ---";
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <pico/stdlib.h>

using namespace std;
using namespace chrono;

enum strengths {
    high_card,
    one_pair,
    two_pair,
    three_of_a_kind,
    full_house,
    four_of_a_kind,
    five_of_a_kind
};

struct hand {
    string h;
    unsigned bid;
    strengths str1, str2;
};

static vector<hand> hands;

static bool found(vector<int> vec, int target) {
    return find(vec.begin(), vec.end(), target) != vec.end();
}

static strengths strength_part1(string hand) {
    map<char, int> counts_map;
    for (auto c : hand) ++counts_map[c];
    vector<int> counts;
    for (auto p : counts_map) counts.push_back(p.second);
    if (found(counts, 5))
        return five_of_a_kind;
    else if (found(counts, 4))
        return four_of_a_kind;
    else if (found(counts, 3)) {
        if (found(counts, 2))
            return full_house;
        else
            return three_of_a_kind;
    } else if (count(counts.begin(), counts.end(), 2) == 2)
        return two_pair;
    else if (found(counts, 2))
        return one_pair;
    else
        return high_card;
}

static map<char, int> strength{{'2', 1},  {'3', 2},  {'4', 3}, {'5', 4}, {'6', 5},
                               {'7', 6},  {'8', 7},  {'9', 8}, {'T', 9}, {'J', 10},
                               {'Q', 11}, {'K', 12}, {'A', 13}};

static bool compare_part1(const hand& a, const hand& b) {
    if (a.str1 != b.str1)
        return a.str1 < b.str1;
    else
        for (int i = 0; i < 5; ++i)
            if (a.h[i] != b.h[i]) return strength[a.h[i]] < strength[b.h[i]];
    return false;
}

static const string faces = "23456789TQKA";

static strengths strength_part2(string hand) {
    set<char> s;
    for (auto c : hand) s.insert(c);
    s.insert('K');
    strengths max_strength = high_card;
    for (auto c : s) {
        string possible = hand;
        replace(possible.begin(), possible.end(), 'J', c);
        max_strength = max(max_strength, strength_part1(possible));
    }
    return max_strength;
}

static map<char, int> wildcard_strength{{'J', 1},  {'2', 2},  {'3', 3}, {'4', 4}, {'5', 5},
                                        {'6', 6},  {'7', 7},  {'8', 8}, {'9', 9}, {'T', 10},
                                        {'Q', 11}, {'K', 12}, {'A', 13}};

static bool compare_part2(const hand& a, const hand& b) {
    if (a.str2 != b.str2)
        return a.str2 < b.str2;
    else
        for (int i = 0; i < 5; ++i)
            if (a.h[i] != b.h[i]) return wildcard_strength[a.h[i]] < wildcard_strength[b.h[i]];
    return false;
}

static unsigned part(int part_n) {
    sort(hands.begin(), hands.end(), part_n == 1 ? compare_part1 : compare_part2);
    unsigned sum = 0;
    for (int i = 0; i < hands.size(); ++i) sum += (i + 1) * hands[i].bid;
    return sum;
}

static const vector<string> lines = {
#include "day07.txt"
};

int main() {
    stdio_init_all();
    auto start = time_us_32();
    string line;
    for (auto& line : lines) {
        stringstream ss(line);
        string hand;
        ss >> hand;
        string s;
        ss >> s;
        unsigned bid = stoll(s);
        hands.push_back({hand, bid, strength_part1(hand), strength_part2(hand)});
    }
    cout << title << endl
         << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(2) << endl
         << "Elapsed - "
         << (time_us_32() - start) / 1000.0
         << " ms." << endl;
}
