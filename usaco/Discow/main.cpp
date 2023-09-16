#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <set>
#include <fstream>
using namespace std;

int find_other(const string &s, const char c, const int a) {
	for (int i = 0; i < 52; i++) {
		if (s[i] == c && i != a) {
			return i;
		}
	}
	return -1;
}

vector<pair<int, int>> pairs(const string& s) {
	vector<pair<int, int>> results;
	for (int i = 0; i < 52; i++) {
		int tmp = find_other(s, s[i], i);
		if (i < tmp) {
			results.push_back(pair<int, int>(i, find_other(s, s[i], i)));
		}
	}
	return results;
}

bool is_overlap(const pair<int, int>& a, const pair<int, int>& b) {
	if ((a.first < b.first && a.second < b.second) && a.second > b.first) {
		return true;
	}
	return false;
}

int main() {
	string s;
	cin >> s;
	auto prs = pairs(s);
	int c = 0;
	for (int i = 0; i < prs.size(); i++) {
		for (int x = 0; x < prs.size(); x++) {
			if (is_overlap(prs[i], prs[x])) {
				c++;
			}
		}
	}
	cout << c << "\n";
}