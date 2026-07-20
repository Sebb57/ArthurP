/**
 * @file Bitboard.hpp
 * @brief board representation for the engine using uint64
 *
 * @author Seb
 * @date 2026-07-16
**/

#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include <cstdint>
#include <string>

namespace engine {

using bitboardType = uint64_t;

enum piecePromotion {
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT
};

enum Piece {
    EMPTY,
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK,
};

struct Move {
    std::string from;
    std::string to;

    piecePromotion promotionPiece = QUEEN;
    bool promotion = false;

    bool capture = false;
    bool castle = false;
};

class Bitboard {
    public:
        Bitboard() = default;
        ~Bitboard() = default;
    
        std::string bitToSquare(int bit);
        int squareToBit(const std::string& square);

        void printBoard(); //debug
        void loadFEN(const std::string& fen);
        void makeMove(const Move& move);

        void setBit(bitboardType& bb, int sq);
        void clearBit(bitboardType& bb, int sq);
        bool getBit(bitboardType bb, int sq);
    private:
        bitboardType pieces[13] = {
            0x0000000000000000ULL, // EMPTY

            0x000000000000FF00ULL, // WP
            0x0000000000000042ULL, // WN
            0x0000000000000024ULL, // WB
            0x0000000000000081ULL, // WR
            0x0000000000000008ULL, // WQ
            0x0000000000000010ULL, // WK

            0x00FF000000000000ULL, // BP
            0x4200000000000000ULL, // BN
            0x2400000000000000ULL, // BB
            0x8100000000000000ULL, // BR
            0x0800000000000000ULL, // BQ
            0x1000000000000000ULL  // BK
        };

        Piece board[64] = {
            WR, WN, WB, WQ, WK, WB, WN, WR,
            WP, WP, WP, WP, WP, WP, WP, WP,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            BP, BP, BP, BP, BP, BP, BP, BP,
            BR, BN, BB, BQ, BK, BB, BN, BR
        };
        bitboardType whitePieces = pieces[WP] | pieces[WN] | pieces[WB] | pieces[WR] | pieces[WQ] | pieces[WK];
        bitboardType blackPieces = pieces[BP] | pieces[BN] | pieces[BB] | pieces[BR] | pieces[BQ] | pieces[BK];
        bitboardType occupied = whitePieces | blackPieces;
        bitboardType empty = ~occupied;
};

}

#endif /* BITBOARD_HPP_ */
