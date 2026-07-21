/**
 * @file MoveGenerator.hpp
 * @brief class responsible for generating every possible move from a bitboard
 *
 * @author Seb
 * @date 2026-07-21
**/

#ifndef MOVEGENERATOR_HPP_
#define MOVEGENERATOR_HPP_

#include <vector>

#include "Bitboard.hpp"

namespace engine {

class MoveGenerator {
    public:
        std::vector<Move> generatePawnMoves(const Bitboard& board, bool whiteToMove) const;
};

}

#endif /* MOVEGENERATOR_HPP_ */
