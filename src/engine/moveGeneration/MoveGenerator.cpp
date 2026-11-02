/**
 * @file MoveGenerator.cpp
 * @brief move generation: pseudo-legal generation per piece, then legality filtering
 *
 * @author Seb
 * @date 2026-07-21
**/

#include "engine/moveGeneration/MoveGenerator.hpp"
#include "engine/Bitboard.hpp"
#include "engine/Types.hpp"
#include "engine/moveGeneration/Attacks.hpp"
#include "engine/moveGeneration/GenFlags.hpp"
#include <algorithm>
#include <bit>
#include <initializer_list>
#include <vector>

namespace engine::movegen {

namespace {

template <Direction D>
void addPawnMoves(bitboardType targets, bitboardType enemies, std::vector<Move>& moves)
{
    constexpr bitboardType promotionRanks = rankBB(0) | rankBB(7);

    for (const Square to : squaresOf(targets)) {
        const Square from = to - D.offset();
        const bool capture = getBit(enemies, to);

        if (!getBit(promotionRanks, to)) {
            moves.push_back(Move{.from = from, .to = to, .capture = capture});
            continue;
        }
        for (const PieceType piece : {QUEEN, ROOK, BISHOP, KNIGHT})
            moves.push_back(Move{.from = from, .to = to, .promotionPiece = piece, .promotion = true, .capture = capture});
    }
}

template <Color Us>
void generatePawnMoves(const Bitboard& board, std::vector<Move>& moves)
{
    constexpr Direction up = Us == WHITE ? NORTH : SOUTH;
    constexpr bitboardType thirdRank = rankBB(Us == WHITE ? 2 : 5);

    const bitboardType pawns = board.getPieceBitboard(Us, PAWN);
    const bitboardType empty = board.getEmpty();
    const bitboardType enemies = board.getColorPieces(~Us);

    const bitboardType singlePushes = shift<up>(pawns) & empty;
    const bitboardType doublePushes = shift<up>(singlePushes & thirdRank) & empty;

    addPawnMoves<up>(singlePushes, enemies, moves);
    addPawnMoves<up + up>(doublePushes, enemies, moves);
    addPawnMoves<up + EAST>(shift<up + EAST>(pawns) & enemies, enemies, moves);
    addPawnMoves<up + WEST>(shift<up + WEST>(pawns) & enemies, enemies, moves);

    const Square enPassant = board.getEnPassantSquare();
    if (enPassant == NO_SQUARE)
        return;
    for (const Square from : squaresOf(PAWN_ATTACKS[~Us][enPassant] & pawns))
        moves.push_back(Move{.from = from, .to = enPassant, .capture = true, .enPassant = true});
}

template <Color Us, PieceType Pt>
void generatePieceMoves(const Bitboard& board, std::vector<Move>& moves)
{
    const bitboardType notOwn = ~board.getColorPieces(Us);
    const bitboardType enemies = board.getColorPieces(~Us);
    const bitboardType occupied = board.getOccupied();

    for (const Square from : squaresOf(board.getPieceBitboard(Us, Pt)))
        for (const Square to : squaresOf(attacks<Pt>(from, occupied) & notOwn))
            moves.push_back(Move{.from = from, .to = to, .capture = getBit(enemies, to)});
}

template <Color Us>
void generateCastlingMoves(const Bitboard& board, std::vector<Move>& moves)
{
    for (const Castling& castling : CASTLINGS[Us]) {
        const bool hasRight = (board.getCastlingRights() & castling.right) != 0;
        const bool isPathEmpty = (board.getOccupied() & castling.mustBeEmpty()) == 0;

        if (hasRight && isPathEmpty)
            moves.push_back(Move{.from = castling.kingFrom, .to = castling.kingTo, .castle = true});
    }
}

template <Color Us>
std::vector<Move> generatePseudoLegalMoves(const Bitboard& board)
{
    std::vector<Move> moves;

    moves.reserve(MAX_MOVES);
    generatePawnMoves<Us>(board, moves);
    generatePieceMoves<Us, KNIGHT>(board, moves);
    generatePieceMoves<Us, BISHOP>(board, moves);
    generatePieceMoves<Us, ROOK>(board, moves);
    generatePieceMoves<Us, QUEEN>(board, moves);
    generatePieceMoves<Us, KING>(board, moves);
    generateCastlingMoves<Us>(board, moves);
    return moves;
}

bool isLegal(const Bitboard& board, const Move& move)
{
    const Color us = board.getSideToMove();

    if (move.castle)
        return std::ranges::none_of(squaresOf(squareRange(move.from, move.to)), [&board, us](Square sq) { return isSquareAttacked(board, sq, ~us); });

    Bitboard next = board;
    next.makeMove(move);
    return !isInCheck(next, us);
}

} // namespace

template <GenFlags Flags>
std::vector<Move> generateMoves(const Bitboard& board)
{
    std::vector<Move> moves = board.getSideToMove() == WHITE ? generatePseudoLegalMoves<WHITE>(board) : generatePseudoLegalMoves<BLACK>(board);

    if constexpr (Flags.includesLegal() != Flags.includesIllegal())
        std::erase_if(moves, [&board](const Move& move) { return isLegal(board, move) != Flags.includesLegal(); });
    return moves;
}
template std::vector<Move> generateMoves<GenFlags{.legal = false, .illegal = false}>(const Bitboard&);
template std::vector<Move> generateMoves<GenFlags{.legal = true, .illegal = false}>(const Bitboard&);
template std::vector<Move> generateMoves<GenFlags{.legal = false, .illegal = true}>(const Bitboard&);
template std::vector<Move> generateMoves<GenFlags{.legal = true, .illegal = true}>(const Bitboard&);

bool isSquareAttacked(const Bitboard& board, Square sq, Color by)
{
    const bitboardType occupied = board.getOccupied();
    const bitboardType queens = board.getPieceBitboard(by, QUEEN);

    return (PAWN_ATTACKS[~by][sq] & board.getPieceBitboard(by, PAWN)) != 0
        || (attacks<KNIGHT>(sq, occupied) & board.getPieceBitboard(by, KNIGHT)) != 0
        || (attacks<KING>(sq, occupied) & board.getPieceBitboard(by, KING)) != 0
        || (attacks<BISHOP>(sq, occupied) & (board.getPieceBitboard(by, BISHOP) | queens)) != 0
        || (attacks<ROOK>(sq, occupied) & (board.getPieceBitboard(by, ROOK) | queens)) != 0;
}

bool isInCheck(const Bitboard& board, Color color)
{
    const bitboardType king = board.getPieceBitboard(color, KING);

    return king != 0 && isSquareAttacked(board, std::countr_zero(king), ~color);
}

} // namespace engine::movegen
