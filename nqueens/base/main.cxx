#include <boost/program_options.hpp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using Index = unsigned short int;
using Placement = std::vector<Index>;

namespace po = boost::program_options;

bool is_valid_placement(const Placement& placement) {
    for (auto ii = 0u; ii < placement.size(); ++ii) {
        auto value = placement[ii];
        for (auto jj = 0u; jj < placement.size(); ++jj) {
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

int main(int argc, char** argv) {
    try {
        // Handle input
        auto n = 0;
        auto description = po::options_description("Allowed options:");
        description.add_options()
                ("help,h", "prints this nice help message")
                ("size,n", po::value<int>(&n)->default_value(8), "size of the board")
                ;

        auto vm = po::variables_map();
        po::store(po::parse_command_line(argc, argv, description), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << description << std::endl;
            return 1;
        }

        // Compute
        auto v = Placement(n);
        std::iota(v.begin(), v.end(), 0);
        auto count = 0;
        do {
            if (is_valid_placement(v)) {
                count++;
            }
        } while (std::next_permutation(v.begin(), v.end()));
        std::cout << "Found " << count << " solution" << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
