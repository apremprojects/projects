#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

struct squirrel_t {
    int C;
    bool start;
    int HH;
    int MM;
};

int operator-(const squirrel_t& a, const squirrel_t& b) {
    int at = (a.HH * 60) + a.MM;
    int bt = (b.HH * 60) + b.MM;
    return at - bt;
}

bool operator<(const squirrel_t& a, const squirrel_t& b) {
    if (a.HH < b.HH) {
        return true;
    }
    else if (a.HH == b.HH) {
        return a.MM < b.MM;
    }
    return false;
}

int cleaning_time(const vector<squirrel_t>& b) {
    int time = 0;
    for (int i = 0; i < b.size(); i += 2) {
        time += b[i + 1] - b[i];
    }
    return time;
}

int main() {
    int N;
    int Nlines;
    cin >> N;
    cin >> Nlines;
    vector<vector<squirrel_t>> squirrels(N);
    for (int i = 0; i < Nlines; i++) {
        squirrel_t squirrel;
        cin >> squirrel.C;
        string s;
        cin >> s;
        squirrel.start = true;
        if (s == "STOP") {
            squirrel.start = false;
        }
        cin >> squirrel.HH;
        cin >> squirrel.MM;
        squirrels[squirrel.C - 1].push_back(squirrel);
    }
    for (auto &i : squirrels) {
        sort(i.begin(), i.end());
        int ct = cleaning_time(i);
        cout << ct / 60 << " " << ct % 60 << "\n";
    }
}