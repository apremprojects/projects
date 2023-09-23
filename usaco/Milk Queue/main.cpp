#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

/*
4
2 3
12 8
6 7
9 2
*/

/*
SORT
2 3
6 7
9 2
12 8
*/

bool operator<(const pair<int, int>& a, const pair<int, int>& b) {
	return a.first < b.first;
}

int main() {
	//ifstream file("../Milk Queue/test.txt");
	int N;
	cin >> N;
	vector<pair<int, int>> pairs(N);
	for (int i = 0; i < N; i++) {
		cin >> pairs[i].first;
		cin >> pairs[i].second;
	}
	sort(pairs.begin(), pairs.end());
	for (int i = 0; i < N - 1; i++) {
		int tmp = pairs[i].first + pairs[i].second;
		//cout << "tmp -> " << tmp << "\n";
		//cout << "pair -> " << pairs[i].first << ", " << pairs[i].second << "\n";
		if (tmp > pairs[i + 1].first) {
			pairs[i + 1].first = tmp;
		}
	}
	cout << pairs[N - 1].first + pairs[N - 1].second << "\n";
}