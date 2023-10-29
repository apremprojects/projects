#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

int main() {
	int F, K;
	cin >> F >> K;
	vector<bool> cows(F);
	vector<pair<int, int>> sf(K);
	for (int i = 0; i < K; i++) {
		cin >> sf[i].first >> sf[i].second;
	}
	for (const auto& p : sf) {
		for (int i = p.first - 1; i < cows.size(); i += p.second) {
			cows[i] = true;
		}
	}
	int unassigned = 0;
	for (const auto i : cows) {
		if (!i) {
			unassigned++;
		}
	}
	cout << unassigned << "\n";
}