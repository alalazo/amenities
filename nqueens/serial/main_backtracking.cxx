#include <boost/program_options.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using Index = short int;
using Placement = std::vector<Index>;

namespace po = boost::program_options;

class NQueensBoard {
public:
    explicit NQueensBoard(Index n) : m_size(n), m_placements(n) {
    }

    NQueensBoard(const NQueensBoard& other) = default;
    
    Index size() const {
        return m_size;
    }
    
    bool is_valid() const {
        auto value = Index(0);
        auto current = Index(0);
        auto d = Index(0);
        for (auto ii = 0; ii < m_current_row; ++ii) {
            value = m_placements[ii];
            for (auto jj = 0; jj < m_current_row; ++jj) {
                d = std::abs(jj - ii); // Distance of the current row from the target value
                current = m_placements[jj];
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
    
    bool already_placed(Index ii) const {
        for (auto jj = 0; jj < m_current_row; ++jj) {
            if (m_placements[jj] == ii) {
                return true;
            }
        }
        return false;
    }
    
    void try_at(Index ii) {
        m_placements[m_current_row] = ii;
        ++m_current_row;
    }
    
    void pop_back() {
        --m_current_row;
    }
    
    bool is_final() const {
        return m_current_row == m_size;
    }
        
    friend std::ostream& operator<<(std::ostream& ss, const NQueensBoard& board);
    
private:
    Index m_size;
    Index m_current_row;
    std::vector<Index> m_placements;
};

std::ostream& operator<<(std::ostream& ss, const NQueensBoard& board) {
    for(auto ii = 0; ii < board.m_size; ++ii) {
        if (ii >= board.m_current_row) {
            ss << "X\t";
        } else {
            ss << board.m_placements[ii] << "\t";
        }
    }    
    return ss;
}

long long unsigned int explore_and_count(NQueensBoard& board){
    
    auto count = 0ull;
#ifndef NDEBUG
    std::cout << board << std::endl;
#endif
    for(auto ii = 0; ii < board.size(); ++ii) {
        if (board.already_placed(ii)) {
            continue;
        }
        board.try_at(ii);
        // If the board is valid
        if (board.is_valid()) {
            if(board.is_final()) {
                // I have reached a valid leaf
#ifndef NDEBUG
                std::cout << board << " **** " << std::endl;
#endif
                ++count;
                
            } else {
               count += explore_and_count(board);
            }
        }
        board.pop_back();
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
