#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>
using namespace std;

int logbase(const int num, const int base) {
	return ceil(log2(num) / log2(base));
}

string convert(const int num, const int base) {
	int size = logbase(num, base);
	int remainder = num;
	//cout << "Final conv_num size -> " << size << "\n";
	string res(size, '0');
	for (int i = size - 1; i >= 0; i--) {
		//cout << "Place -> " << pow(base, i) << "\n";
		//cout << remainder / int(pow(base, i)) << " " << pow(base,i) << "'s\n";
		res[size-i - 1] = (remainder / int(pow(base, i))) + '0';
		remainder -= (remainder / int(pow(base, i))) * pow(base, i);
	}
	return res;
}

int main() {
	int num;
	int base;
	cin >> num >> base;
	cout << convert(num, base);
}