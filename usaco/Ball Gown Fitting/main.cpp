#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

int main() {
	int N, S;
	cin >> N >> S;
	vector<int> foxes(N);
	for (int i = 0; i < N; i++) {
		cin >> foxes[i];
	}
	sort(foxes.begin(), foxes.end());
	int count = 0;
	for (int i = 0; i < N; i++) {
		int b_max = upper_bound(foxes.begin(), foxes.end(), S - foxes[i]) - foxes.begin();
		if (b_max - (i + 1) >= 0) {
			count += b_max - (i + 1);
		}
		
	}
	cout << count << "\n";
}