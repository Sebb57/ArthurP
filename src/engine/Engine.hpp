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
#include "engine/MoveGenerator.hpp"

namespace engine {

class Engine {
    public:
        std::string search(int n, bool whiteToMove);

        void loadFen(const std::string& fen) { this->board.loadFEN(fen); }   
        void makeMove(const Move& move) { this->board.makeMove(move); }
        Piece getPieceAt(const std::string& square) const { return this->board.getPieceAt(square); }
    private:
        Bitboard board;
        MoveGenerator generator;
        // Evaluator eval; // taking a bitboard and evaluates it (one public method that takes bitboard and returns int)
};

}

#endif /* ENGINE_HPP_ */
