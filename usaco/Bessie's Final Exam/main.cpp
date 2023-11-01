#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <bitset>
#include <iostream>

using namespace std;

bool fullAdder(bool b1, bool b2, bool& carry)
{
    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

bitset<8000> operator+(const bitset<8000>& a, const bitset<8000>& b) {
    bool carry = false;
    // bitset to store the sum of the two bitsets 
    bitset<8000> ans;
    for (int i = 0; i < 8000; i++) {
        ans[i] = fullAdder(a[i], b[i], carry);
    }
    return ans;
}

int main() {
    string binary;
    cin >> binary;
    bitset<8000> b_orig(binary);
    bitset<8000> b;
    for (int i = 0; i < 17; i++) {
        b = b + b_orig;
    }
    int i = 0;
    string s = b.to_string();
    for (; i < s.size(); i++) {
        if (s[i] == '1') {
            break;
        }
    }
    cout << s.substr(i) << "\n";
}