#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>
using namespace std;

bool overlap(const pair<int, int> &bl_1, const pair<int, int> &tr_1, const pair<int, int> &bl_2, const pair<int, int> &tr_2) {
	if (bl_1.first == tr_1.first || bl_1.second == tr_1.second || tr_2.first == bl_2.first || bl_2.second == tr_2.second) {
		return false;
	}

	// If one rectangle is on left side of other
	if (bl_1.first > tr_2.first || bl_2.first > tr_1.first) {
		return false;
	}

	// If one rectangle is above other
	if (tr_1.second > bl_2.second || tr_2.second > bl_1.second) {
		return false;
	}

	return true;
}

int main() {
	int x1_1, y1_1, x2_1, y2_1, x1_2, y1_2, x2_2, y2_2;
	cin >> x1_1 >> y1_1 >> x2_1 >> y2_1 >> x1_2 >> y1_2 >> x2_2 >> y2_2;
	x1_1 += 1000;
	y1_1 += 1000;
	x2_1 += 1000;
	y2_1 += 1000;

	x1_2 += 1000;
	y1_2 += 1000;
	x2_2 += 1000;
	y2_2 += 1000;

	vector<vector<bool>> arr(2001, vector<bool>(2001, true));

	for (int x = x1_1; x <= x2_1; x++) {
		for (int y = y1_1; y <= y2_1; y++) {
			arr[y][x] = false;
		}
	}

	for (int x = x1_2; x <= x2_2; x++) {
		for (int y = y1_2; y <= y2_2; y++) {
			arr[y][x] = true;
		}
	}

	int min_x = 2000;
	int min_y = 2000;
	int max_x = 0;
	int max_y = 0;
	for (int y = 0; y < 2001; y++) {
		for (int x = 0; x < 2001; x++) {
			if (!arr[y][x] && x < min_x) {
				min_x = x;
			}
			else if (!arr[y][x] && x > max_x) {
				max_x = x;
			}

			if (!arr[y][x] && y < min_y) {
				min_y = y;
			}
			else if (!arr[y][x] && y > max_y) {
				max_y = y;
			}
		}
	}
	cout << (max_x - min_x) * (max_y - min_y) << "\n";
}