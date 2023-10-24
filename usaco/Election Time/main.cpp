#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <set>
#include <fstream>
using namespace std;

int min_index(const vector<int>& one_votes, const int u_start) {
	int min_ind = u_start;
	int min = one_votes[u_start];
	for (int i = u_start; i < one_votes.size(); i++) {
		if (one_votes[i] < min) {
			min = one_votes[i];
			min_ind = i;
		}
	}
	return min_ind;
}

void special_sort(int u_start, vector<int> &one_votes, vector<int> &ordering) {
	for (; u_start < one_votes.size(); u_start++) {
		int min_ind = min_index(one_votes, u_start);
		swap(one_votes[u_start], one_votes[min_ind]);
		swap(ordering[u_start], ordering[min_ind]);
	}
}

void generate_ordering(vector<int>& ordering) {
	for (int i = 0; i < ordering.size(); i++) {
		ordering[i] = i;
	}
}

int main() {
	int N, K;
	cin >> N;
	cin >> K;
	vector<int> one_votes(N);
	vector<int> two_votes(N);
	vector<int> ordering(N);
	generate_ordering(ordering);
	for (int i = 0; i < N; i++) {
		cin >> one_votes[i] >> two_votes[i];
	}
	special_sort(0, one_votes, ordering);

	vector<int> two_can(K);
	for (int i = 0; i < K; i++) {
		two_can[i] = ordering[N - i - 1];
	}

	int mx = 0;
	int mx_i = 0;
	for (int i = 0; i < two_can.size(); i++) {
		if (two_votes[two_can[i]] > mx) {
			mx = two_votes[two_can[i]];
			mx_i = two_can[i];
		}
	}
	cout << mx_i + 1 << "\n";
}