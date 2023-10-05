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
int main() {
    int N;
    cin >> N;
    vector<int> nums(N);
    for (int i = 0; i < N; i++) {
        cin >> nums[i];
    }
    sort(nums.begin(), nums.end());
    vector<bool> dir(N, true);
    int ans = 0;
    vector<int> fr(N, 0);
    for (int i = 1; i <= N - 2; i++) {
        int left_dist = nums[i] - nums[i - 1];
        int right_dist = nums[i + 1] - nums[i];
        dir[i] = false; //false - left, true - right
        if (right_dist < left_dist) {
            dir[i] = true;
        }
    }
    dir[0] = true;
    dir[N - 1] = false;
    if (!dir[1]) {
        fr[0] = 1;
    }
    if (dir[N - 2]) {
        fr[N - 1] = 1;
    }
    for (int i = 1; i < N - 1; i++) {
        if (dir[i - 1]) {
            fr[i]++;
        }
        if(!dir[i + 1]){
            fr[i]++;
        }
    }
    for (int i = 0; i < N; i++) {
        if (fr[i] == 0) {
            ans++;
        }
    }
    for (int i = 0; i < N - 1; i++) {
        if (fr[i + 1] == 1 && fr[i] == 1 && dir[i + 1] == false && dir[i] == true) {
            ans++;
        }
    }
    cout << "\n";
    for (int i : fr) {
        cout << i << " ";
    }
    cout << "\n";
    for (bool b : dir) {
        cout << b << " ";
    }
    cout << ans << "\n";
}