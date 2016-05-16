#include <boost/program_options.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using Index = unsigned short int;
using Placement = std::vector<Index>;

namespace po = boost::program_options;

class NQueensBoard {
public:
    explicit NQueensBoard(Index n) : m_to_be_placed(n, 0) {
        std::iota(m_to_be_placed.begin(), m_to_be_placed.end(), 0);
        m_placements.reserve(n);        
    }

    NQueensBoard(const NQueensBoard& other) = default;
    
    bool is_valid() const {
        for (auto ii = 0u; ii < m_placements.size(); ++ii) {
            auto value = m_placements[ii];
            for (auto jj = 0u; jj < m_placements.size(); ++jj) {
                auto d = std::abs(jj - ii); // Distance of the current row from the target value
                auto current = m_placements[jj];
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
    
    void try_remaining_at(Index ii) {
        m_placements.push_back(m_to_be_placed[ii]);
        m_to_be_placed.erase( m_to_be_placed.begin() + ii );
    }
    
    bool is_final() const {
        return m_to_be_placed.empty();
    }
    
    const std::vector<Index> & remaining_positions() const {
        return m_to_be_placed;
    }
    
    friend std::ostream& operator<<(std::ostream& ss, const NQueensBoard& board);
    
private:
    std::vector<Index> m_to_be_placed;
    std::vector<Index> m_placements;
};

std::ostream& operator<<(std::ostream& ss, const NQueensBoard& board) {
    for(const auto& x : board.m_placements){
        ss << x << "\t";
    }
    std::for_each(board.m_to_be_placed.cbegin(), board.m_to_be_placed.cend(),
    [&](const Index& x){
         ss << "X\t";
    }
    );
    return ss;
}

long long unsigned int explore_and_count(const NQueensBoard& current){
    
    auto count = 0ull;
    auto remaining = current.remaining_positions();
#ifndef NDEBUG
    std::cout << current << std::endl;
#endif
    for(auto ii = 0u; ii < remaining.size(); ++ii) {
        auto next = NQueensBoard(current);
        next.try_remaining_at(ii);
        // If the board is valid
        if (next.is_valid()) {
            if(next.is_final()) {
                // I have reached a valid leaf
#ifndef NDEBUG
                std::cout << next << " **** " << std::endl;
#endif
                count += 1;
                continue;
            } else {
               count += explore_and_count(next);
            }
        }
    }
    return count;
}


int main(int argc, char** argv) {
    try {
        // Handle input
        auto n = 0;
        auto description = po::options_description("Allowed options:");
        description.add_options()
                ("help,h", "prints this nice help message")
                ("size,n", po::value<int>(&n)->default_value(4), "size of the board")
                ;

        auto vm = po::variables_map();
        po::store(po::parse_command_line(argc, argv, description), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << description << std::endl;
            return 1;
        }

        // Compute
        auto board = NQueensBoard(n);
        auto count = explore_and_count(board);
        std::cout << "Found " << count << " solution" << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
