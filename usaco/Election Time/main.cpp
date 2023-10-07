#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <set>
#include <fstream>
using namespace std;

vector<int> shuffler(vector<int> ids, const vector<int> &shuffle) {
    vector<int> result(ids.size());
    for (int i = 0; i < ids.size(); i++) {
        //result[shuffle[i] - 1] = ids[i];
        result[i] = ids[shuffle[i] - 1];
    }
    return result;
}

int main() {
    int N;
    cin >> N;
    vector<int> shuffle(N);
    vector<int> ids(N);
    for (int i = 0; i < N; i++) {
        cin >> shuffle[i];
    }
    for (int i = 0; i < N; i++) {
        cin >> ids[i];
    }
    for (int i = 0; i < 3; i++) {
        ids = shuffler(ids, shuffle);
    }
    for (auto i : ids) {
        cout << i << "\n";
    }
}