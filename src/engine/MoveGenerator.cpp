/**
 * @file MoveGenerator.cpp
 * @brief move generation helpers
 *
 * @author Seb
 * @date 2026-07-21
**/

#include "MoveGenerator.hpp"

namespace {

void addPromotionMoves(std::vector<engine::Move>& moves, const std::string& from, const std::string& to)
{
    for (engine::piecePromotion promotion : { engine::QUEEN, engine::ROOK, engine::BISHOP, engine::KNIGHT }) {
        engine::Move move;
        move.from = from;
        move.to = to;
        move.promotion = true;
        move.promotionPiece = promotion;
        moves.push_back(move);
    }
}

}

std::vector<engine::Move> engine::MoveGenerator::generatePawnMoves(const Bitboard& board, bool whiteToMove) const
{
    std::vector<Move> moves;
    const Piece pawnPiece = whiteToMove ? WP : BP;
    const bitboardType pawns = board.getPieceBitboard(pawnPiece);
    const bitboardType occupied = board.getOccupied();
    const int forward = whiteToMove ? 8 : -8;
    const int doubleForward = whiteToMove ? 16 : -16;
    const int startRank = whiteToMove ? 1 : 6;
    const int promotionRank = whiteToMove ? 6 : 1;

    for (int sq = 0; sq < 64; ++sq) {
        if (!board.getBit(pawns, sq))
            continue;

        const int rank = sq / 8;
        const int oneStep = sq + forward;

        if (oneStep >= 0 && oneStep < 64 && !board.getBit(occupied, oneStep)) {
            if (rank == promotionRank) {
                addPromotionMoves(moves, board.bitToSquare(sq), board.bitToSquare(oneStep));
            } else {
                Move move;
                move.from = board.bitToSquare(sq);
                move.to = board.bitToSquare(oneStep);
                moves.push_back(move);

                const int twoStep = sq + doubleForward;
                if (rank == startRank && twoStep >= 0 && twoStep < 64 && !board.getBit(occupied, twoStep)) {
                    Move doublePush;
                    doublePush.from = board.bitToSquare(sq);
                    doublePush.to = board.bitToSquare(twoStep);
                    moves.push_back(doublePush);
                }
            }
        }
    }

    return moves;
}
