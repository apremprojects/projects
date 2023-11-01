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
	for (int i = 0; i < N; i++) {
		total += times[i];
		times[i] = total;
	}
	
	for (auto q : questions) {
		cout << 1 + (std::upper_bound(times.begin(), times.end(), q) - times.begin()) << "\n";
	}
}