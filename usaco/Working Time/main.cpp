#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

int main() {
	int N, Q;
	cin >> N >> Q;
	vector<int> times(N);
	vector<int> questions(Q);
	for (int i = 0; i < N; i++) {
		cin >> times[i];
	}
	for (int i = 0; i < Q; i++) {
		cin >> questions[i];
	}
	int total = 0;
	for (int i = 0; i < times.size(); i++) {
		total += times[i];
		times[i] = total;
	}
	for (const auto question : questions) {
		int val = int(upper_bound(times.begin(), times.end(), question) - times.begin());
		cout << val + 1 << "\n";
	}
}