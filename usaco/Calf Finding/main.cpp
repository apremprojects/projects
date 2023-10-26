#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <fstream>
#include <cmath>
using namespace std;

bool is_between(int old_pos, const int y, int pos) {
	if (old_pos > pos) {
		swap(pos, old_pos);
	}
	return (old_pos <= y) && (y <= pos);
}

int main() {
	int x = 0;
	int y = 0;
	cin >> x >> y;
	int i = 0;
	int dist = 0;
	int pos = x;
	int old_pos = x;
	while (true) {
		pos = x + pow(-2, i);
		if (is_between(old_pos, y, pos)) {
			dist += abs(y - old_pos);
			cout << dist << "\n";
			return 0;
		}
		dist += abs(old_pos - pos);
		i++;
		old_pos = pos;
	}
}