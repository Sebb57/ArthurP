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
#include "engine/Types.hpp"
#include "eval/Eval.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace engine {

class Engine {
    public:
        [[nodiscard]] std::string search(int n) const;

        void loadFen(std::string_view fen) { this->board.loadFEN(fen); }
        void makeMove(const Move& move) { this->board.makeMove(move); }
        [[nodiscard]] std::vector<Move> getLegalMoves() const;
    private:
        Bitboard board;
        eval::Evaluator eval;
};

} // namespace engine

#endif /* ENGINE_HPP_ */
