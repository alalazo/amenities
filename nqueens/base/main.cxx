#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <tuple>

using Index = unsigned short int;
using Placement = std::vector<Index>;

bool is_valid_placement(const Placement& placement) {
	for (auto ii = 0; ii < placement.size(); ++ii) {
		auto value = placement[ii];
		for (auto jj = 0; jj < placement.size(); ++jj) {
			auto d = std::abs(jj - ii); // Distance of the current row from the target value
			auto current = placement[jj];
			if (d == 0) { // Don't compare a row with itself
				continue;
			}
			if ((value + d == current) || (value - d == current)) {
				return false;
			}
		}
	}
	return true;
}

int main() {

	auto v = Placement(11);
	std::iota(v.begin(), v.end(), 0);
	auto count = 0;
	do {
		if (is_valid_placement(v)) {
			count++;
		}
	} while (std::next_permutation(v.begin(), v.end()));
	std::cout << "Found " << count << " solution" << std::endl;
	return 0;
}
