#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

void layer() {

}

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

void fill_grid(vector<bool> &lawn, const int X, const int Y, const pair<int, int> &a, const pair<int, int> &b) {
    for (int x = a.first - 1; x < b.first; x++) {
        for (int y = a.second - 1; y < b.second; y++) {
            cout << x << ',' << y << '\n';
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
    //cout << "Grid : \n";
    //disp_grid(lawn, X, Y);
    cout << "Filling : \n";
    for (const pair<pair<int, int>, pair<int, int>>& sp : layering_commands) {
        pair<int, int> a = sp.first;
        pair<int, int> b = sp.second;
        fill_grid(lawn, X, Y, a, b);
        disp_grid(lawn, X, Y);
    }
}