#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <set>
#include <fstream>
using namespace std;

struct Cow {
	int p = -1;
	int m = -1;
	int t = -1;
};

bool operator<(const Cow &a, const Cow &b) {
	if (a.p < b.p) {
		return true;
	}
	else if (a.p == b.p) {
		return a.t < b.t;
	}
	else {
		return false;
	}
}

vector<int> mix(vector<int> a, vector<int> b) {
	if (a.size() > b.size()) {
		swap(a, b);
	}
	vector<int> res;
	sort(a.begin(), a.end());
	for (int i : b) {
		int index = std::lower_bound(a.begin(), a.end(), i) - a.begin();
		if (a[index] == i) {
			res.push_back(i);
		}
	}
	return res;
}

int main() {
	int N, M, D, S;
	cin >> N >> M >> D >> S;
	vector<Cow> cows(D);
	vector<pair<int, int>> times(S);
	for (int i = 0; i < D; i++) {
		Cow cow;
		cin >> cow.p;
		cin >> cow.m;
		cin >> cow.t;
		cows[i] = cow;
	}
	for (int i = 0; i < S; i++) {
		cin >> times[i].first >> times[i].second;
	}
	std::sort(cows.begin(), cows.end());
	//cout << "Sorted : \n";
	/*for (const Cow& c : cows) {
		cout << c.p << ", " << c.m << ", " << c.t << "\n";
	}*/
	vector<vector<int>> vcows;
	for (const pair<int, int>& p : times) {
		Cow c;
		c.p = p.first;
		int start = lower_bound(cows.begin(), cows.end(), c) - cows.begin();
		vector<int> vicows;
		int i = start;
		while (cows[i].p == p.first && cows[i].t < p.second) {
			vicows.push_back(cows[i].m);
			i++;
		}
		vcows.push_back(vicows);
		//cout << "Fields Cow #" << p.first << " has visited : \n";
		/*for (const int i : vicows) {
			cout << i << "\n";
		}*/
	}
	vector<int> result = vcows[0];
	for (int i = 1; i < vcows.size(); i++) {
		result = mix(result, vcows[i]);
	}
	std::set<int> results;
	sort(result.begin(), result.end());
	for (const Cow& cow : cows) {
		if (result[lower_bound(result.begin(), result.end(), cow.m) - result.begin()] == cow.m) {
			results.emplace(cow.p);
		}
	}
	cout << results.size() << "\n";
}