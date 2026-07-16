/**
 * @file Engine.hpp
 * @brief engine class holding everything needed to generate the best move
 *
 * @author Seb
 * @date 2026-07-16
**/

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include "engine/Bitboard.hpp"

namespace engine {

class Engine {
    public:
        void loadFen(const std::string& fen) { this->board.loadFEN(fen); }   
    private:
        Bitboard board;
        // MoveGenerator generator; // taking a bitboard as param and returns an array of moves
        // Evaluator eval; // taking a bitboard and evaluates it (one public method that takes bitboard and returns int)
};

}

#endif /* ENGINE_HPP_ */

