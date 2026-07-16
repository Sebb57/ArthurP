/**
 * @file Bitboard.cpp
 * @brief bitboard methods
 *
 * @author Seb
 * @date 2026-07-16
**/

#include "Bitboard.hpp"
#include <string>

int engine::Bitboard::squareToBit(const std::string& square)
{
    char file = square[0];
    char rank = square[1];
    int fileIndex = file - 'a';
    int rankIndex = rank - '1';

    return rankIndex * 8 + fileIndex;
}

std::string engine::Bitboard::bitToSquare(int bit)
{
    char file = 'a' + (bit % 8);
    char rank = '1' + (bit / 8);

    return std::string{file, rank};
}

void setBit(uint64_t& bb, int sq)
{
    bb |= 1ULL << sq;
}

void clearBit(uint64_t& bb, int sq)
{
    bb &= ~(1ULL << sq);
}

bool getBit(uint64_t bb, int sq)
{
    return bb & (1ULL << sq);
}

void engine::Bitboard::makeMove(const Move& move)
{
    int from = squareToBit(move.from);
    int to   = squareToBit(move.to);
    Piece moving = board[from];

    if (moving == EMPTY)
        return;
    clearBit(pieces[moving], from);

    if (move.capture)
        clearBit(pieces[board[to]], to);
    setBit(pieces[moving], to);

    board[to] = moving;
    board[from] = EMPTY;
    whitePieces = pieces[WP] | pieces[WN] | pieces[WB] | pieces[WR] | pieces[WQ] | pieces[WK];
    blackPieces = pieces[BP] | pieces[BN] | pieces[BB] | pieces[BR] | pieces[BQ] | pieces[BK];
    occupied = whitePieces | blackPieces;
    empty = ~occupied;
}

void engine::Bitboard::loadFEN(const std::string& fen)
{
    (void) fen;
}
