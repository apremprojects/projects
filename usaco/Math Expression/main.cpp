#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> Ss;
    vector<int> Os;
    vector<int> Ms;
    vector<int> Es;
    vector<int> Bs;
    vector<int> Is;
    vector<int> Gs;
    for (int i = 0; i < N; i++) {
        char c;
        cin >> c;
        int num;
        cin >> num;
        if (c == 'S') {
            Ss.push_back(num);
        }
        else if (c == 'O') {
            Os.push_back(num);
        }
        else if (c == 'M') {
            Ms.push_back(num);
        }
        else if (c == 'E') {
            Es.push_back(num);
        }
        else if (c == 'B') {
            Bs.push_back(num);
        }
        else if (c == 'I') {
            Is.push_back(num);
        }
        else if (c == 'G') {
            Gs.push_back(num);
        }
    }
    int count = 0;
    for(const int S: Ss){
        for (const int O : Os) {
            for (const int M : Ms) {
                for (const int E : Es) {
                    for (const int B : Bs) {
                        for (const int I : Is) {
                            for (const int G : Gs) {
                                int num = (O + M + O) * (S + I + S + B + E + E) * (E + G + O + S);
                                if (num % 2 == 0) {
                                    count++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    cout << count << "\n";
}