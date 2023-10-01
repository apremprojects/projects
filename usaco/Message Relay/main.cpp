#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

/*
7
4 2 8 11 16 3 24
*/

/*
SORT
7
2 3 4 8 11 16 24
    *         *
SORTED DISTANCE
1 1 4 3 5 8
*/

vector<int> dist(const vector<int>& nums) {
    vector<int> vdist(nums.size() - 1);
    for (int i = 1; i < nums.size(); i++) {
        vdist[i - 1] = nums[i] - nums[i - 1];
    }
    return vdist;
}

int main() {
    int N;
    cin >> N;
    vector<int> nums(N);
    for (int i = 0; i < N; i++) {
        cin >> nums[i];
    }
    sort(nums.begin(), nums.end());
    /*cout << "Sorted : \n";
    for (int num : nums) {
        cout << num << " ";
    }*/
    //cout << "\n";
    vector<int> vdist = dist(nums);
    for (const int d : vdist) {
        cout << d << "\n";
    }
    cout << "Result : \n";
    int c = 0;
    for (int i = 1; i < vdist.size(); i++) {
        if (vdist[i] > vdist[i - 1]) {
            c++;
        }
    }
    cout << N - 2 - c << "\n";
}