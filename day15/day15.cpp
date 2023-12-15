constexpr auto title = "--- Day 15: Lens Library ---";

#include <pico/stdlib.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

unsigned char hash_string(string instruction) {
    unsigned char result = 0;
    for (unsigned char c : instruction) result = (result + c) * 17;
    return result;
}

unsigned int part1(string& line) {
    size_t start = 0;
    size_t end = line.find(',');
    unsigned int sum = 0;
    while (end != string::npos) {
        sum += hash_string(line.substr(start, end - start));
        start = end + 1;
        end = line.find(',', start);
    }
    return sum;
}

int part2(string& line) {
    size_t start = 0;
    size_t end = line.find(',');
    vector<vector<pair<string, int>>> boxes(256);
    while (end != string::npos) {
        string instruction = line.substr(start, end - start);
        if (instruction.find("-") != string::npos) {
            string label = instruction.substr(0, instruction.size() - 1);
            boxes[hash_string(label)].erase(
                remove_if(boxes[hash_string(label)].begin(), boxes[hash_string(label)].end(),
                          [&label](const pair<string, int>& p) { return p.first == label; }),
                boxes[hash_string(label)].end());
        } else {
            string label = instruction.substr(0, instruction.find('='));
            int level = stoi(instruction.substr(instruction.find('=') + 1));
            vector<pair<string, int>>& target = boxes[hash_string(label)];
            bool replaced = false;
            for (auto& l : target)
                if (l.first == label) {
                    l.second = level;
                    replaced = true;
                }
            if (!replaced) target.push_back({label, level});
        }
        start = end + 1;
        end = line.find(',', start);
    }
    int sum = 0;
    for (int i = 0; i < boxes.size(); i++)
        for (int j = 0; j < boxes[i].size(); j++) sum += (i + 1) * (j + 1) * boxes[i][j].second;
    return sum;
}

static string line =
#include "day15.txt"
    ;

int main() {
    stdio_init_all();
    auto start = time_us_32();
    cout << title << endl
         << "Part 1  - " << part1(line) << endl
         << "Part 2  - " << part2(line) << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
