#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

void backtrack(vector<int>& nums, const int v) {
	for (int i = v - nums[v]; i < v; i++) {
		nums[i] = i + nums[v] - v;
		if (nums[i] != -1) {
			cout << -1;
			exit(0);
		}
	}
}

void fillIn(vector<int>& nums) {
	for (int i = 0; i < nums.size(); i++) {
		if (i != -1 || i != 0) {
			backtrack(nums, i);
		}
	}
}

void disp(const vector<int>& nums) {
	for (auto i : nums) {
		if (i != -1) {
			cout << i;
		}
		else {
			cout << "?";
		}
		cout << " ";
	}
	cout << "\n";
}

int main() {
	int N;
	cin >> N;
	vector<int> nums(N);
	for (int i = 0; i < N; i++) {
		cin >> nums[i];
	}
	fillIn(nums);
	int min = 0;
	int max = 0;
	for (int i = 0; i < nums.size(); i++) {
		if (nums[i] == 0) {
			min++;
			max++;
		}
		else if(nums[i] == -1) {
			max++;
		}
	}
	cout << min << " " << max << "\n";
}