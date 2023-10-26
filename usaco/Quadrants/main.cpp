#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>
using namespace std;

vector<int> gen_sort_x(vector<pair<int, int>> &pairs) {
	vector<int> x(pairs.size());
	auto x_sort = [](const pair<int, int> &a, const pair<int, int> &b) {
		return a.first < b.first;
	};
	sort(pairs.begin(), pairs.end(), x_sort);
	for (int i = 0; i < pairs.size(); i++) {
		x[i] = pairs[i].first;
	}
	return x;
}

vector<int> gen_sort_y(vector<pair<int, int>>& pairs) {
	vector<int> y(pairs.size());
	auto y_sort = [](const pair<int, int>& a, const pair<int, int>& b) {
		return a.second < b.second;
	};
	sort(pairs.begin(), pairs.end(), y_sort);
	for (int i = 0; i < pairs.size(); i++) {
		y[i] = pairs[i].second;
	}
	return y;
}

int upper_left_quadrant(const vector<pair<int, int>> &pairs, const int x_line, const int y_line) {
	int c = 0;
	for (auto& i : pairs) {
		if (i.first < x_line && i.second > y_line) {
			c++;
		}
	}
	return c;
}

int upper_right_quadrant(const vector<pair<int, int>>& pairs, const int x_line, const int y_line) {
	int c = 0;
	for (auto& i : pairs) {
		if (i.first > x_line && i.second > y_line) {
			c++;
		}
	}
	return c;
}

int bottom_left_quadrant(const vector<pair<int, int>>& pairs, const int x_line, const int y_line) {
	int c = 0;
	for (auto& i : pairs) {
		if (i.first < x_line && i.second < y_line) {
			c++;
		}
	}
	return c;
}

int bottom_right_quadrant(const vector<pair<int, int>>& pairs, const int x_line, const int y_line) {
	int c = 0;
	for (auto& i : pairs) {
		if (i.first > x_line && i.second < y_line) {
			c++;
		}
	}
	return c;
}

int main() {
	int N, B;
	cin >> N >> B;
	vector<pair<int, int>> pairs(N);
	for (int i = 0; i < N; i++) {
		cin >> pairs[i].first >> pairs[i].second;
	}
	vector<int> x = gen_sort_x(pairs);
	vector<int> y = gen_sort_y(pairs);
	int smallest_max_quadrant = 10000;
	for (int i = 1; i < pairs.size(); i++) {
		int x_line = (x[i - 1] + x[i]) / 2;
		for (int b = 1; b < pairs.size(); b++) {
			int y_line = (y[b - 1] + y[b]) / 2;
			int tl = upper_left_quadrant(pairs, x_line, y_line);
			int tr = upper_right_quadrant(pairs, x_line, y_line);
			int bl = bottom_left_quadrant(pairs, x_line, y_line);
			int br = bottom_right_quadrant(pairs, x_line, y_line);
			int tmax = std::max(tl, std::max(tr, std::max(bl, br)));
			if (tmax < smallest_max_quadrant && tl + tr + bl + br == pairs.size()) {
				smallest_max_quadrant = tmax;
			}
		}
	}
	cout << smallest_max_quadrant << "\n";
}