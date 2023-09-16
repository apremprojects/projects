#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <set>
#include <fstream>
using namespace std;

set<char> get_unique(const vector<string> &cows, const int d) {
    set<char> r;
    for (const string& s : cows) {
        r.insert(s[d]);
    }
    return r;
}

bool increment(const set<char> &bc, const set<char> &brc) {
    for (char i : bc) {
        for (char r : brc) {
            if (i == r) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int N, M;
    cin >> N;
    cin >> M;
    int c = 0;
    vector<string> black_cows(N);
    vector<string> brown_cows(N);
    for (int i = 0; i < N; i++) {
        cin >> black_cows[i];
    }
    for (int i = 0; i < N; i++) {
        cin >> brown_cows[i];
    }
    for (int d = 0; d < M; d++) {
        set<char> bc = get_unique(black_cows, d);
        set<char> brc = get_unique(brown_cows, d);
        if (increment(bc, brc)) {
            c++;
        }
    }
    cout << c << "\n";
}