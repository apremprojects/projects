#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <set>
#include <fstream>
using namespace std;

enum class ITEM {
	HORN,
	GRASS,
	SHEARS
};

ITEM getItem(const int a, const int horn_symbol, const int grass_symbol, const int shear_symbol) {
	if (a == horn_symbol) {
		return ITEM::HORN;
	}
	else if (a == grass_symbol) {
		return ITEM::GRASS;
	}
	else {
		return ITEM::SHEARS;
	}
}

bool is_greater(const ITEM &a, const ITEM &b) {
	if (a == ITEM::HORN) {
		if (b == ITEM::SHEARS) {
			return true;
		}
	}
	else if (a == ITEM::GRASS) {
		if (b == ITEM::HORN) {
			return true;
		}
	}
	else if (a == ITEM::SHEARS) {
		if(b == ITEM::GRASS){
			return true;
		}
	}
	return false;
}

int getWins(const vector<pair<int, int>> &cows, const int horn_symbol, const int grass_symbol, const int shear_symbol) {
	int wins = 0;
	for (int i = 0; i < cows.size();i++) {
		ITEM a = getItem(cows[i].first, horn_symbol, grass_symbol, shear_symbol);
		ITEM b = getItem(cows[i].second, horn_symbol, grass_symbol, shear_symbol);
		if (is_greater(a, b)) {
			wins++;
		}
	}
	return wins;
}

int main() {
	int N;
	cin >> N;
	vector<pair<int, int>> cows(N);
	for (int i = 0; i < N; i++) {
		cin >> cows[i].first >> cows[i].second;
	}
	int onetwothree = getWins(cows, 1, 2, 3);
	int onethreetwo = getWins(cows, 1, 3, 2);
	int twoonethree = getWins(cows, 2, 1, 3);
	int twothreeone = getWins(cows, 2, 3, 1);
	int threeonetwo = getWins(cows, 3, 1, 2);
	int threetwoone = getWins(cows, 3, 2, 1);
	cout << std::max(onetwothree, std::max(onethreetwo, std::max(twoonethree, std::max(twothreeone, std::max(threeonetwo, threetwoone)))));
}