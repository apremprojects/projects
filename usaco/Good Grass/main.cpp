#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>
using namespace std;

int main() {
	vector<bool> arr(2001 * 2001, true);
	//false = square bessie can eat
	//true = square bessie CAN'T eat
	int x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4, x_5, y_5, x_6, y_6;
	cin >> x_1 >> y_1 >> x_2 >> y_2 >> x_3 >> y_3 >> x_4 >> y_4 >> x_5 >> y_5 >> x_6 >> y_6;
	x_1 += 1000;
	y_1 += 1000;
	x_2 += 1000;
	y_2 += 1000;

	x_3 += 1000;
	y_3 += 1000;
	x_4 += 1000;
	y_4 += 1000;

	x_5 += 1000;
	y_5 += 1000;
	x_6 += 1000;
	y_6 += 1000;
	for (int x = x_1; x < x_2; x++) {
		for (int y = y_1; y < y_2; y++) {
			arr[(y * 2000) + x] = false;
		}
	}
	for (int x = x_3; x < x_4; x++) {
		for (int y = y_3; y < y_4; y++) {
			arr[(y * 2000) + x] = false;
		}
	}
	for (int x = x_5; x < x_6; x++) {
		for (int y = y_5; y < y_6; y++) {
			arr[(y * 2000) + x] = true;
		}
	}
	int ec = 0;
	for (const bool i : arr) {
		if (!i) {
			ec++;
		}
	}
	cout << ec << "\n";
}