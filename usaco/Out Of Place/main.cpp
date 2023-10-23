#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <fstream>
using namespace std;

struct cow_t {
    string name;
    int round;
    int point_increase;
};

void display_leatherboard(const int round_number, const int mildred, const int elsie, const int bessie) {
    cout << "Round #" << round_number << "\n";
    cout << "Mildred score -> " << mildred << "\n";
    cout << "Elsie score -> " << elsie << "\n";
    cout << "Bessie score -> " << bessie << "\n";
}

bool operator<(const cow_t& a, const cow_t& b) {
    return a.round < b.round;
}

int main() {
    int c = 0;
    int N;
    ifstream file("../MooTimes/test.txt");
    file >> N;
    vector<cow_t> cows(N);
    for (int i = 0; i < N; i++) {
        cow_t cow;
        file >> cow.round;
        file >> cow.name;
        file >> cow.point_increase;
        cows[i] = cow;
    }
    sort(cows.begin(), cows.end());
    array<int, 3> score_breed{7, 7, 7};
    vector<int> leader{1, 2, 3};
    int max_score = 7;
    for (int i = 0; i < N; i++) {
        if (cows[i].name == "Mildred") {
            score_breed[0] += cows[i].point_increase;
        }
        else if (cows[i].name == "Elsie") {
            score_breed[1] += cows[i].point_increase;
        }
        else if (cows[i].name == "Bessie") {
            score_breed[2] += cows[i].point_increase;
        }
        vector<int> curr_leader;
        int curr_max_score = std::max(score_breed[0], std::max(score_breed[1], score_breed[2]));
        for (int i = 0; i < 3; i++) {
            if (score_breed[i] == curr_max_score) {
                curr_leader.push_back(i);
            }
        }
        if (curr_max_score != max_score && curr_leader != leader) {
            c++;
            max_score = curr_max_score;
            leader = curr_leader;
        }
        else if (curr_max_score == max_score && curr_leader.size() != leader.size()) {
            c++;
            leader = curr_leader;
        }
    }
    cout << c << "\n";
}