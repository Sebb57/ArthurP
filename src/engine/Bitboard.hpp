/**
 * @file Bitboard.hpp
 * @brief board representation for the engine using uint64
 *
 * @author Seb
 * @date 2026-07-16
**/

#ifndef BITBOARD_HPP_
#define BITBOARD_HPP_

#include "engine/Types.hpp"
#include <array>
#include <string_view>

namespace engine {

class Bitboard {
    public:
        Bitboard();

        void printBoard() const; //debug
        void loadFEN(std::string_view fen);
        void makeMove(const Move& move);

        [[nodiscard]] Piece getPieceAt(Square sq) const { return board[sq]; }

        [[nodiscard]] bitboardType getPieceBitboard(Piece piece) const { return pieces[piece]; }
        [[nodiscard]] bitboardType getPieceBitboard(Color color, PieceType type) const { return pieces[makePiece(color, type)]; }
        [[nodiscard]] bitboardType getColorPieces(Color color) const { return colorPieces[color]; }
        [[nodiscard]] bitboardType getOccupied() const { return occupied; }
        [[nodiscard]] bitboardType getEmpty() const { return ~occupied; }

        [[nodiscard]] Color getSideToMove() const { return sideToMove; }
        [[nodiscard]] CastlingRights getCastlingRights() const { return castlingRights; }
        [[nodiscard]] Square getEnPassantSquare() const { return enPassantSquare; }

    private:
        void clear();
        void putPiece(Piece piece, Square sq);
        void removePiece(Square sq);
        void movePiece(Square from, Square to);

        std::array<bitboardType, PIECE_COUNT> pieces{};
        std::array<bitboardType, COLOR_COUNT> colorPieces{};
        bitboardType occupied = 0;
        std::array<Piece, SQUARE_COUNT> board{};

        Color sideToMove = WHITE;
        CastlingRights castlingRights = NO_CASTLING;
        Square enPassantSquare = NO_SQUARE;
};

} // namespace engine

#endif /* BITBOARD_HPP_ */
