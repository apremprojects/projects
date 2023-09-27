#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

char symbol(bool sym) {
    if (sym) {
        return '#';
    }
    else {
        return '.';
    }
}

void disp_grid(const vector<bool> &lawn, const int X, const int Y) {
    for (int i = 0; i < X * Y; i++) {
        cout << symbol(lawn[i]) << ' ';
        if (i % X == X - 1) {
            cout << '\n';
        }
    }
}

void fill_cell(vector<bool> &lawn, const int x, const int y, const int X, const int Y) {
    lawn[(y * X) + x] = true;
}

void fill_grid(vector<bool> &lawn, const int X, const int Y, pair<int, int> a, pair<int, int> b) {
    int x_one = a.first - 1;
    int x_two = b.first - 1;
    int y_one = Y - a.second;
    int y_two = Y - b.second;
    if (x_one > x_two) {
        swap(x_one, x_two);
    }
    if (y_one > y_two) {
        swap(y_one, y_two);
    }
    //fill_cell(lawn, x_one, y_one, X, Y);
    //fill_cell(lawn, x_two, y_two, X, Y);
    for (int x = x_one; x <= x_two; x++) {
        for (int y = y_one; y <= y_two; y++) {
            fill_cell(lawn, x, y, X, Y);
        }
    }
}

int main() {
    int X, Y, A;
    cin >> X;
    cin >> Y;
    cin >> A;
    vector<bool> lawn(X * Y, false); //false - no sod, true - sod
    vector<pair<pair<int, int>, pair<int, int>>> layering_commands(A);
    for (int i = 0; i < A; i++) {
        pair<int, int> a;
        pair<int, int> b;
        cin >> a.first;
        cin >> a.second;
        cin >> b.first;
        cin >> b.second;
        layering_commands[i].first = a;
        layering_commands[i].second = b;
    }
    for (const pair<pair<int, int>, pair<int, int>>& sp : layering_commands) {
        pair<int, int> a = sp.first;
        pair<int, int> b = sp.second;
        fill_grid(lawn, X, Y, a, b);
    }
    int count = 0;
    for (const bool b : lawn) {
        if (b) {
            count++;
        }
    }
    cout << count << "\n";
}