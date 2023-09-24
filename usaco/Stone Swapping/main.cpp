#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

vector <string> ninety_clockwise(vector<string> start) {
    int size = start.size();
    int layer_count = size / 2;
    for (int layer = 0; layer < layer_count; layer++) {
        int first = layer;
        int last = size - first - 1;
        for (int element = first; element < last; element++) {
            int offset = element - first;
            int top = start[first][element];
            int right_side = start[element][last];
            int bottom = start[last][last - offset];
            int left_side = start[last - offset][first];
            start[first][element] = left_side;
            start[element][last] = top;
            start[last][last - offset] = right_side;
            start[last - offset][first] = bottom;
        }
    }
    return start;
}

vector<string> one_hundred_and_eighty_degrees_clockwise(vector<string> start) {
    return ninety_clockwise(ninety_clockwise(start));
}

vector<string> two_hundred_and_seventy_degrees_clockwise(vector<string> start) {
    return ninety_clockwise(ninety_clockwise(ninety_clockwise(start)));
}

vector<string> horz_reflect(vector<string> start) {
    vector<string> result(start.size(), string(start.size(), ' '));
    for (int layer = 0; layer < start.size(); layer++) {
        for (int i = 0; i < start.size(); i++) {
            result[layer][i] = start[layer][start.size() - i - 1];
        }
    }
    return result;
}

void disp(const vector<string>& d) {
    for (auto s: d) {
        cout << s << "\n";
    }
}

int main() {
    int N;
    //ifstream file("../Stone Swapping/test.txt");
    //file >> N;
    cin >> N;
    vector<string> start(N);
    vector<string> result(N);
    vector<int> nums;
    for (int i = 0; i < N; i++) {
        cin >> start[i];
    }
    for (int i = 0; i < N; i++) {
        cin >> result[i];
    }
    //cout << "Start:\n";
    //disp(start);
    //cout << "Result:\n";
    //disp(result);
    auto nc = ninety_clockwise(start);
    //cout << "90 Clockwise:\n";
    //disp(nc);
    auto oc = one_hundred_and_eighty_degrees_clockwise(start);
    //cout << "180 Clockwise:\n";
    //disp(oc);
    auto tc = two_hundred_and_seventy_degrees_clockwise(start);
    //cout << "270 Clockwise:\n";
    //disp(tc);
    auto hr = horz_reflect(start);
    //cout << "Horizontally reflected:\n";
    //disp(hr);
    if (result == nc) {
        nums.push_back(1);
    }
    if (result == oc) {
        nums.push_back(2);
    }
    if (result == tc) {
        nums.push_back(3);
    }
    if (result == hr) {
        nums.push_back(4);
    }
    auto tmp = hr;
    for (int i = 0; i < 3; i++) {
        tmp = ninety_clockwise(tmp);
        if (tmp == result) {
            nums.push_back(5);
        }
    }
    if (result == start) {
        nums.push_back(6);
    }
    if (nums.size() == 0) {
        nums.push_back(7);
    }
    sort(nums.begin(), nums.end());
    cout << nums[0] << "\n";
}