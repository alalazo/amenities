#include <boost/program_options.hpp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include <cmath>

namespace po = boost::program_options;

int main(int argc, char** argv) {
    // Handle input
    try {
        auto n = 0;
        auto print = false;
        auto description = po::options_description("Allowed options");
        description.add_options()
                ("help,h", "prints this nice help message")
                ("size,n", po::value<int>(&n)->default_value(8), "maximum number to search for (excluded)")
                ("print,p", po::value<bool>(&print)->default_value(false), "whether to print the numbers or not")
                ;

        auto vm = po::variables_map();
        po::store(po::parse_command_line(argc, argv, description), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << description << std::endl;
            return 1;
        }

        // Compute
        auto v = std::vector<bool>(n, true);
        v[0] = false;
        for (auto ii = 2u; ii < std::sqrt(n) + 1; ++ii) {
            if (v[ii]) { // If the number is prime
                for (auto jj = 2 * ii; jj < v.size(); jj += ii) {
                    v[jj] = false;
                }
            }
        }
        if (print) {
            for (auto ii = 0u; ii < v.size(); ii++) {
                if (v[ii]) {
                    std::cout << ii << std::endl;
                }
            }
        } else {
            auto last_index = std::find(v.rbegin(), v.rend(), true) - v.rbegin();
            last_index = v.size() - 1 - last_index;
            std::cout << "Found " << std::count(v.cbegin(), v.cend(), true);
            std::cout << " prime numbers [last : " << last_index << " ]" << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
