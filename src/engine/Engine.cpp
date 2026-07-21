/**
 * @file Engine.cpp
 * @brief engine algorithms to find best move
 *
 * @author Seb
 * @date 2026-07-21
**/

#include "Engine.hpp"
#include <algorithm>
#include <string>
#include <vector>

namespace {

char promotionToUciChar(engine::piecePromotion promotion)
{
    switch (promotion) {
        case engine::QUEEN:  return 'q';
        case engine::ROOK:   return 'r';
        case engine::BISHOP: return 'b';
        case engine::KNIGHT: return 'n';
        default:             return '\0';
    }
}

std::string moveToText(const engine::Move& move)
{
    std::string text = move.from + move.to;
    if (move.promotion) {
        const char promo = promotionToUciChar(move.promotionPiece);
        if (promo != '\0')
            text.push_back(promo);
    }
    return text;
}

}

std::string engine::Engine::search(int n, bool whiteToMove)
{
    (void)n;
    const std::vector<Move> moves = this->generator.generatePawnMoves(this->board, whiteToMove);

    if (moves.empty())
        return "";
    return moveToText(moves.front());
}
