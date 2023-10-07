#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

int min_index(const vector<int>& nums, const int u_start) {
	int min = nums[u_start];
	int min_ind = u_start;
	for (int i = u_start; i < nums.size(); i++) {
		if (nums[i] < min) {
			min = nums[i];
			min_ind = i;
		}
	}
	return min_ind;
}

int special_sort(vector<int>& nums, vector<pair<int, int>> &ps) {
	int count = 0;
	for (int u_start = 0; u_start < nums.size(); u_start++) {
		int min_ind = min_index(nums, u_start);
		if (min_ind != u_start) {
			swap(nums[min_ind], nums[u_start]);
			cout << u_start << " " << min_ind << "\n";
			count++;
		}
	}
	return count;
}

int main() {
	int N;
	cin >> N;
	vector<int> nums(N);
	vector<pair<int, int>> ps(N, pair<int, int>(0, 0));
	for (int i = 0; i < N; i++) {
		cin >> nums[i];
	}
	cout << special_sort(nums, ps);
}