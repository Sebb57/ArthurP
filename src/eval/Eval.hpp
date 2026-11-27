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
    int evaluate(const engine::Bitboard& board, const engine::Move& move) const {
        engine::Bitboard copy = board;
        const engine::Color us = copy.getSideToMove();
        int score = 0;

        copy.makeMove(move);
        for (const auto piece : copy.getAllPieces()) {
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

