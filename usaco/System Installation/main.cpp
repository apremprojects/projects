#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

int char_to_int(const char c) {
    return c - 'a';
}

void unique_letters(const string &s, vector<int> &l) {
    for (const char c : s) {
        l[char_to_int(c)] += 1;
    }
}

vector<int> get_letters(const string& s) {
    vector<int> l(26, 0);
    for (const char c : s) {
        l[char_to_int(c)]++;
    }
    return l;
}

vector<vector<int>> transpose(const vector<vector<int>> data) {
    vector<vector<int> > result(data[0].size(),
        vector<int>(data.size()));
    for (vector<int>::size_type i = 0; i < data[0].size(); i++)
        for (vector<int>::size_type j = 0; j < data.size(); j++) {
            result[i][j] = data[j][i];
        }
    return result;
}

vector<int> operator+(const vector<int> &a, const vector<int> &b) {
    vector<int> result(26, 0);
    for (int i = 0; i < 26; i++) {
        result[i] = a[i] + b[i];
    }
    return result;
}

char int_to_char(const int i) {
    return 'a' + i;
}

void disp(const vector<int> &v) {
    for (int i = 0; i < 26; i++) {
        if (v[i] != 0) {
            cout << int_to_char(i) << "x" << v[i] << " ";
        }
    }
    cout << "\n";
}

int calculate_digit(const int d, const vector<vector<int>>& vletters) {
    int c = 0;
    for (int r = 0; r < vletters.size(); r += 2) {
        c += std::max(vletters[r][d], vletters[r + 1][d]);
    }
    return c;
}

int main() {
    int N;
    ifstream file("../Picture Game/test.txt");
    file >> N;
    //cin >> N;
    vector<string> words(2 * N);
    for (int i = 0; i < 2 * N; i++) {
        file >> words[i];
        //cin >> words[i];
    }
    vector<vector<int>> vletters(2 * N);
    for (int i = 0; i < 2 * N; i++) {
        vletters[i] = get_letters(words[i]);
    }
    vector<int> result(26, 0);
    /*for (auto letters : vletters) {
        for (auto letter: letters) {
            cout << letter << " ";
        }
        result = result + letters;
        cout << "\n";
        disp(letters);
    }*/
    for (int i = 0; i < 26; i++) {
        result[i] = calculate_digit(i, vletters);
    }
    //cout << "answer: \n";
    for (auto r : result) {
        cout << r << " ";
    }
    cout << "\n";
}