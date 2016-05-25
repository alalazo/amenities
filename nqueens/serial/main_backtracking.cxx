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
    explicit NQueensBoard(Index n) : 
    m_size(n), m_placements(n), m_placed(n, false)
    , m_diagonal_left(2*n, false), m_diagonal_right(2*n, false) {
    }

    NQueensBoard(const NQueensBoard& other) = default;

    Index size() const {
        return m_size;
    }
    
    bool is_invalid_placement(Index ii) const {
        auto left = ii - m_current_row + m_size;
        auto right = ii + m_current_row;
        
        auto under_threat = ( 
        m_placed[ii] || // Same row or column
        m_diagonal_left[left] || // Threat from left diagonal
        m_diagonal_right[right] // Threat from right diagonal
        );
        
        return under_threat;
    }
    
    void try_at(Index ii) {
        // Place the value of the column in the current row
        m_placements[m_current_row] = ii;
        // Mark the horizontal and vertical place holders
        m_placed[ii] = true;
        // Mark the diagonal place holders
        auto left = ii - m_current_row + m_size;
        auto right = ii + m_current_row;
        m_diagonal_left[left] = true;
        m_diagonal_right[right] = true;        
        // Proceed to the next row
        ++m_current_row;        
    }
    
    void pop_back(Index ii) {
        --m_current_row;
        // Un-mark horizontal and vertical place holders
        m_placed[ii] = false;
        // Un-mark the diagonal place holders
        auto left = ii - m_current_row + m_size;
        auto right = ii + m_current_row;
        m_diagonal_left[left] = false;
        m_diagonal_right[right] = false;        
    }
    
    bool is_final() const {
        return m_current_row == m_size;
    }
        
    friend std::ostream& operator<<(std::ostream& ss, const NQueensBoard& board);
    
private:
    Index m_size;
    Index m_current_row;
    std::vector<Index> m_placements;
    std::vector<bool> m_placed;
    std::vector<bool> m_diagonal_left;
    std::vector<bool> m_diagonal_right;
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

long long unsigned int explore_and_count(NQueensBoard& board) {

    auto count = 0ull;
#ifndef NDEBUG
    std::cout << board << std::endl;
#endif
    for (auto ii = 0; ii < board.size(); ++ii) {
        if (board.is_invalid_placement(ii)) {
            continue;
        }
        board.try_at(ii);
        if (board.is_final()) { // I have reached a valid leaf
#ifndef NDEBUG
            std::cout << board << " **** " << std::endl;
#endif
            ++count;
        } else {
            count += explore_and_count(board);
        }
        board.pop_back(ii);
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
