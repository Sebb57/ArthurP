/**
 * @file Eval.hpp
 * @brief position evaluator class
 *
 * @author Seb
 * @date 2026-11-27
**/

#ifndef EVAL_HPP_
#define EVAL_HPP_

#include "engine/Bitboard.hpp"
#include "engine/Types.hpp"
#include <array>

namespace eval {

class Evaluator {
public:
    Evaluator() = default;
    ~Evaluator() = default;

    // temporary function as long as i don't have a real evaluation function
    [[nodiscard]] int evaluate(const engine::Bitboard& board) const {
        const engine::Color us = board.getSideToMove();
        int score = 0;

        for (const auto piece : board.getAllPieces()) {
            const int value = engine::pieceValue(piece);
            if (value == 0)
                continue;
            score += (engine::pieceColor(piece) == us) ? value : -value;
        }
        return score;
    }
};

} // namespace eval

#endif /* EVAL_HPP_ */

