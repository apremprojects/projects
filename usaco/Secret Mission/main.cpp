#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

size_t char_to_int(const char c) {
    return c - 'A' + 1;
}

size_t get_code(const string &s) {
    size_t rv = 1;
    for (const char c : s) {
        rv *= char_to_int(c);
    }
    return rv % 47;
}

int main() {
    string cow;
    string code;
    cin >> cow;
    cin >> code;
    if (get_code(cow) != get_code(code)) {
        cout << "STAY\n";
    }
    else {
        cout << "GO\n";
    }
}