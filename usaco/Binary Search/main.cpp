#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

int lowerBound(const vector<int> nums,const int num) {
	int low = 0;
	int high = nums.size();
	int mid; //declare the variables
	while (low < high) {
		mid = (low + high) / 2;
		if (num <= nums[mid]){
			//move high to mid
			high = mid;
		}
		else {
			low = mid + 1;
			//move low to mid + 1
		}
	}
	if (nums[low] == num) {
		return low;
	}
	else {
		return -1;
	}
	return -1;
}

int main() {
	int N, Q;
	cin >> N >> Q;
	vector<int> nums(N);
	vector<int> queries(Q);
	for (int i = 0; i < N; i++) {
		cin >> nums[i];
	}
	for (int i = 0; i < Q; i++) {
		cin >> queries[i];
	}
	for (auto query : queries) {
		cout << lowerBound(nums, query) << "\n";
	}
}