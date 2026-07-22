/**
 * @file MoveGenerator.cpp
 * @brief move generation helpers
 *
 * @author Seb
 * @date 2026-07-21
**/

#include "MoveGenerator.hpp"
#include "engine/Bitboard.hpp"
#include <vector>

namespace {

bool kingInCheck(const engine::Bitboard& board, bool whiteToMove)
{
    (void) board; 
    (void)whiteToMove;
    return false;
}

bool isLegalAfterMove(const engine::Bitboard& board, const engine::Move& move, bool whiteToMove)
{
    engine::Bitboard nextBoard = board;

    nextBoard.makeMove(move);
    return !kingInCheck(nextBoard, whiteToMove);
}

} // anonymous namespace 

std::vector<engine::Move> engine::MoveGenerator::generateLegalMoves(const Bitboard& board, bool whiteToMove) const
{
    std::vector<engine::Move> moves;

    this->generatePawnMoves(board, whiteToMove, moves);

    return moves;
}

std::vector<engine::Move> engine::MoveGenerator::generatePawnMoves(
    const Bitboard& board, 
    bool whiteToMove, 
    std::vector<Move>& moves) const
{
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
                for (engine::piecePromotion promotion : { engine::QUEEN, engine::ROOK, engine::BISHOP, engine::KNIGHT }) {
                    Move move;
                    move.from = board.bitToSquare(sq);
                    move.to = board.bitToSquare(oneStep);
                    move.promotion = true;
                    move.promotionPiece = promotion;

                    if (isLegalAfterMove(board, move, whiteToMove))
                        moves.push_back(move);
                }
            } else {
                Move move;
                move.from = board.bitToSquare(sq);
                move.to = board.bitToSquare(oneStep);
                if (isLegalAfterMove(board, move, whiteToMove))
                    moves.push_back(move);

                const int twoStep = sq + doubleForward;
                if (rank == startRank && twoStep >= 0 && twoStep < 64 && !board.getBit(occupied, twoStep)) {
                    Move doublePush;
                    doublePush.from = board.bitToSquare(sq);
                    doublePush.to = board.bitToSquare(twoStep);
                    if (isLegalAfterMove(board, doublePush, whiteToMove))
                        moves.push_back(doublePush);
                }
            }
        }
    }

    return moves;
}
