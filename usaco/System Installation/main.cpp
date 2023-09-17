#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

int get_val(const vector<int>& nums, const int NC, const int r, const int c) {
    return nums[(r * NC) + c];
}

int get_grid_val(const vector<int>& nums, const int NC, const int r, const int c) {
    int total = 0;
    for (int ac = 0; ac < 3; ac++) {
        for (int ar = 0; ar < 3; ar++) {
            total += get_val(nums, NC, r + ar, c + ac);
        }
    }
    return total;
}

int main() {
    int NR, NC;
    cin >> NR >> NC;
    vector<int> nums(NR * NC);
    for (int i = 0; i < NR * NC; i++) {
        cin >> nums[i];
    }
    int max = 0;
    pair<int, int> answer(0, 0);
    for (int c = 0; c < NC - 3; c++) {
        for (int r = 0; r < NR - 3; r++) {
            int tmax = get_grid_val(nums, NC, r, c);
            if (tmax > max) {
                max = tmax;
                answer.first = r;
                answer.second = c;
            }
        }
    }
    cout << max << "\n";
    cout << answer.first + 1 << " " << answer.second + 1 << "\n";
}