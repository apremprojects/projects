#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <set>
#include <fstream>
using namespace std;

void snext(int& count) {
	if (count < 9) {
		count++;
	}
	else {
		count = 1;
	}
}

void disp_digit(const vector<vector<int>> &triangle, const int d) {
	for (int i = 0; i < triangle.size(); i++) {
		if (d < triangle[i].size()) {
			cout << triangle[i][d] << " ";
		}
		else {
			cout << "  ";
		}
	}
}

int main() {
	int N, S;
	cin >> N;
	cin >> S;
	vector<vector<int>> triangle(N);
	int count = S;
	for (int i = 1; i < N + 1; i++) {
		vector<int> tmp(i);
		for (int r = 0; r < i; r++) {
			tmp[r] = count;
			snext(count);
		}
		triangle[i - 1] = tmp;
	}
	for (int i = 0; i < N; i++) {
		disp_digit(triangle, i);
		cout << "\n";
	}
}