#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

bool operator<(const pair<int, int> &a, const pair<int, int> &b) {
	if (a.first < b.first) {
		return true;
	}
	else if(a.first == b.first){
		return a.second < b.second;
	}
	return false;
}

pair<int, int> operator-(const pair<int, int>& a, const pair<int, int>& b) {
	return pair<int, int>(a.first - b.first, a.second - b.second);
}

bool op_x(const pair<int, int>& a, const pair<int, int>& b) {
	return a.first < b.first;
}

bool op_y(const pair<int, int>& a, const pair<int, int>& b) {
	return a.second < b.second;
}

int get_max_x(vector<pair<int, int>>& points) {
	int max_x = -100000;
	for (const auto& i : points) {
		if (i.first > max_x) {
			max_x = i.first;
		}
	}
	return max_x;
}

int get_min_x(vector<pair<int, int>>& points) {
	int min_x = 100000;
	for (const auto& i : points) {
		if (i.first < min_x) {
			min_x = i.first;
		}
	}
	return min_x;
}

int get_max_y(vector<pair<int, int>>& points) {
	int max_y = -100000;
	for (const auto& i : points) {
		if (i.first > max_y) {
			max_y = i.second;
		}
	}
	return max_y;
}

int get_min_y(vector<pair<int, int>>& points) {
	int min_y = 100000;
	for (const auto& i : points) {
		if (i.first < min_y) {
			min_y = i.second;
		}
	}
	return min_y;
}

int calc_area(vector<pair<int, int>>& points, const int ignore_index) {
	int width, height;
	if (ignore_index == -1) {
		width = get_max_x(points) - get_min_x(points);
		height = get_max_y(points) - get_min_y(points);
	}
	else {
		swap(points[ignore_index], points[points.size() - 1]);
		sort(points.begin(), points.end() - 1, op_x);
		width = points[points.size() - 2].first - points[0].first;
		sort(points.begin(), points.end() - 1, op_y);
		height = points[points.size() - 2].second - points[0].second;
	}
	return width * height;
}

int main() {
	int N;
	cin >> N;
	vector<pair<int, int>> points(N);
	for (int i = 0; i < N; i++) {
		cin >> points[i].first >> points[i].second;
	}

	sort(points.begin(), points.end(), op_x);
	int width = points.back().first - points.front().first;
	sort(points.begin(), points.end(), op_y);
	int height = points.back().second - points.front().second;
	sort(points.begin(), points.end(), op_x);
	//ignore rightmost point
	int irp = calc_area(points, points.size() - 1);
	//ignore leftmost point
	sort(points.begin(), points.end(), op_x);
	int ilp =  calc_area(points, 0);

	sort(points.begin(), points.end(), op_y);
	//ignore topmost point
	int itp = calc_area(points, points.size() - 1);
	//ignore bottommost point
	sort(points.begin(), points.end(), op_y);
	int ibp = calc_area(points, 0);
	cout << std::min(irp, std::min(ilp, std::min(itp, ibp))) << "\n";
}