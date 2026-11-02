/**
 * @file Types.hpp
 * @brief core chess types shared by the board and the move generator
 *
 * @author Seb
 * @date 2026-11-02
**/

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace engine {

inline constexpr std::string_view START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
inline constexpr std::size_t MAX_MOVES = 256;

// ---------------------------------------------------------------------------
// Squares and bitboards
// ---------------------------------------------------------------------------
using bitboardType = std::uint64_t;
using Square = int;
inline constexpr int SQUARE_COUNT = 64;
inline constexpr Square NO_SQUARE = -1;

enum SquareName : std::uint8_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
};

[[nodiscard]] constexpr int fileOf(Square sq) { return sq % 8; }
[[nodiscard]] constexpr int rankOf(Square sq) { return sq / 8; }
[[nodiscard]] constexpr Square makeSquare(int file, int rank) { return rank * 8 + file; }
[[nodiscard]] constexpr bool isOnBoard(int file, int rank) { return file >= 0 && file < 8 && rank >= 0 && rank < 8; }
[[nodiscard]] constexpr Square squareToBit(std::string_view square) { return makeSquare(square[0] - 'a', square[1] - '1'); }
[[nodiscard]] constexpr std::string bitToSquare(Square sq) { return {static_cast<char>('a' + fileOf(sq)), static_cast<char>('1' + rankOf(sq))}; }
[[nodiscard]] constexpr bitboardType squareBB(Square sq) { return 1ULL << static_cast<unsigned>(sq); }
[[nodiscard]] constexpr bitboardType fileBB(int file) { return 0x0101010101010101ULL << static_cast<unsigned>(file); }
[[nodiscard]] constexpr bitboardType rankBB(int rank) { return 0xFFULL << (8U * static_cast<unsigned>(rank)); }
constexpr void setBit(bitboardType& bb, Square sq) { bb |= squareBB(sq); }
constexpr void clearBit(bitboardType& bb, Square sq) { bb &= ~squareBB(sq); }
[[nodiscard]] constexpr bool getBit(bitboardType bb, Square sq) { return (bb & squareBB(sq)) != 0; }
[[nodiscard]] constexpr bitboardType squareRange(Square a, Square b)
{
    bitboardType range = 0;

    for (Square sq = std::min(a, b); sq <= std::max(a, b); sq++)
        setBit(range, sq);
    return range;
}

class SquareIterator {
    public:
        using value_type = Square;
        using difference_type = std::ptrdiff_t;

        constexpr SquareIterator() = default;
        constexpr explicit SquareIterator(bitboardType bits) : remaining(bits) {}

        [[nodiscard]] constexpr Square operator*() const { return std::countr_zero(remaining); }
        constexpr SquareIterator& operator++()
        {
            remaining &= remaining - 1;
            return *this;
        }
        constexpr SquareIterator operator++(int)
        {
            const SquareIterator previous = *this;
            ++*this;
            return previous;
        }
        [[nodiscard]] constexpr bool operator==(const SquareIterator&) const = default;

    private:
        bitboardType remaining = 0;
};

struct SquareRange {
    bitboardType bits = 0;

    [[nodiscard]] constexpr SquareIterator begin() const { return SquareIterator{bits}; }
    [[nodiscard]] static constexpr SquareIterator end() { return SquareIterator{}; }
};
[[nodiscard]] constexpr SquareRange squaresOf(bitboardType bb) { return SquareRange{.bits = bb}; }

// ---------------------------------------------------------------------------
// Colors and pieces
// ---------------------------------------------------------------------------
enum Color : std::uint8_t { WHITE, BLACK };
inline constexpr int COLOR_COUNT = 2;
[[nodiscard]] constexpr Color operator~(Color color) { return color == WHITE ? BLACK : WHITE; }
enum PieceType : std::uint8_t { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
inline constexpr int PIECE_TYPE_COUNT = 6;
enum Piece : std::uint8_t {
    EMPTY,
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK,
};
inline constexpr int PIECE_COUNT = 13;

[[nodiscard]] constexpr Piece makePiece(Color color, PieceType type) { return static_cast<Piece>(static_cast<int>(WP) + type + (color * PIECE_TYPE_COUNT)); }
[[nodiscard]] constexpr Color pieceColor(Piece piece) { return piece >= BP ? BLACK : WHITE; }
[[nodiscard]] constexpr PieceType pieceType(Piece piece) { return static_cast<PieceType>((piece - WP) % PIECE_TYPE_COUNT); }

// ---------------------------------------------------------------------------
// Castling
// ---------------------------------------------------------------------------
using CastlingRights = unsigned;
inline constexpr CastlingRights NO_CASTLING = 0U;
inline constexpr CastlingRights WHITE_KINGSIDE = 1U << 0U;
inline constexpr CastlingRights WHITE_QUEENSIDE = 1U << 1U;
inline constexpr CastlingRights BLACK_KINGSIDE = 1U << 2U;
inline constexpr CastlingRights BLACK_QUEENSIDE = 1U << 3U;

struct Castling {
    CastlingRights right = NO_CASTLING;
    Square kingFrom = NO_SQUARE;
    Square kingTo = NO_SQUARE;
    Square rookFrom = NO_SQUARE;
    Square rookTo = NO_SQUARE;

    [[nodiscard]] constexpr bitboardType mustBeEmpty() const { return squareRange(kingFrom, rookFrom) & ~squareBB(kingFrom) & ~squareBB(rookFrom); }
};

inline constexpr std::array<std::array<Castling, 2>, COLOR_COUNT> CASTLINGS = {
    std::array{
        Castling{.right = WHITE_KINGSIDE,  .kingFrom = E1, .kingTo = G1, .rookFrom = H1, .rookTo = F1},
        Castling{.right = WHITE_QUEENSIDE, .kingFrom = E1, .kingTo = C1, .rookFrom = A1, .rookTo = D1},
    },
    std::array{
        Castling{.right = BLACK_KINGSIDE,  .kingFrom = E8, .kingTo = G8, .rookFrom = H8, .rookTo = F8},
        Castling{.right = BLACK_QUEENSIDE, .kingFrom = E8, .kingTo = C8, .rookFrom = A8, .rookTo = D8},
    },
};

[[nodiscard]] constexpr CastlingRights castlingRightsLost(Square sq)
{
    CastlingRights lost = NO_CASTLING;

    for (const auto& colorCastlings : CASTLINGS)
        for (const Castling& castling : colorCastlings)
            if (sq == castling.kingFrom || sq == castling.rookFrom)
                lost |= castling.right;
    return lost;
}

// ---------------------------------------------------------------------------
// Moves
// ---------------------------------------------------------------------------
struct Move {
    Square from = NO_SQUARE;
    Square to = NO_SQUARE;

    PieceType promotionPiece = QUEEN;
    bool promotion = false;

    bool capture = false;
    bool castle = false;
    bool enPassant = false;

    [[nodiscard]] constexpr bool operator==(const Move&) const = default;
};

[[nodiscard]] constexpr std::string moveToText(const Move& move)
{
    constexpr std::string_view pieceLetters = "pnbrqk";
    std::string text = bitToSquare(move.from) + bitToSquare(move.to);

    if (move.promotion)
        text.push_back(pieceLetters[move.promotionPiece]);
    return text;
}

} // namespace engine

#endif /* TYPES_HPP_ */
