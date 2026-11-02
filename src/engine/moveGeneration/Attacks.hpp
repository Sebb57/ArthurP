/**
 * @file Attacks.hpp
 * @brief compile-time attack tables and sliding piece attacks
 *
 * @author Seb
 * @date 2026-11-02
**/

#ifndef ATTACKS_HPP_
#define ATTACKS_HPP_

#include "engine/Types.hpp"
#include <array>
#include <bit>
#include <cstddef>

namespace engine::movegen {

struct Direction {
    int fileDelta = 0;
    int rankDelta = 0;

    [[nodiscard]] constexpr int offset() const { return rankDelta * 8 + fileDelta; }
    [[nodiscard]] constexpr Direction operator+(const Direction& other) const
    {
        return {.fileDelta = fileDelta + other.fileDelta, .rankDelta = rankDelta + other.rankDelta};
    }
};

inline constexpr Direction NORTH{.fileDelta = 0, .rankDelta = 1};
inline constexpr Direction SOUTH{.fileDelta = 0, .rankDelta = -1};
inline constexpr Direction EAST{.fileDelta = 1, .rankDelta = 0};
inline constexpr Direction WEST{.fileDelta = -1, .rankDelta = 0};
inline constexpr Direction NORTH_EAST = NORTH + EAST;
inline constexpr Direction NORTH_WEST = NORTH + WEST;
inline constexpr Direction SOUTH_EAST = SOUTH + EAST;
inline constexpr Direction SOUTH_WEST = SOUTH + WEST;

[[nodiscard]] constexpr Square step(Square sq, Direction dir)
{
    const int file = fileOf(sq) + dir.fileDelta;
    const int rank = rankOf(sq) + dir.rankDelta;

    return isOnBoard(file, rank) ? makeSquare(file, rank) : NO_SQUARE;
}

template <Direction D>
[[nodiscard]] constexpr bitboardType shift(bitboardType bb)
{
    static_assert(D.fileDelta >= -1 && D.fileDelta <= 1, "a shift cannot cross more than one file");

    if constexpr (D.fileDelta == 1)
        bb &= ~fileBB(7);
    else if constexpr (D.fileDelta == -1)
        bb &= ~fileBB(0);

    if constexpr (D.offset() > 0)
        return bb << static_cast<unsigned>(D.offset());
    else
        return bb >> static_cast<unsigned>(-D.offset());
}

namespace detail {

template <std::size_t N>
[[nodiscard]] constexpr std::array<bitboardType, SQUARE_COUNT> leaperAttacks(const std::array<Direction, N>& steps)
{
    std::array<bitboardType, SQUARE_COUNT> table{};

    for (Square sq = 0; sq < SQUARE_COUNT; ++sq)
        for (const Direction& dir : steps)
            if (const Square to = step(sq, dir); to != NO_SQUARE)
                setBit(table[sq], to);
    return table;
}

} // namespace detail

inline constexpr std::array<bitboardType, SQUARE_COUNT> KNIGHT_ATTACKS = detail::leaperAttacks(std::array{
    NORTH + NORTH_EAST, NORTH + NORTH_WEST, SOUTH + SOUTH_EAST, SOUTH + SOUTH_WEST,
    EAST + NORTH_EAST, EAST + SOUTH_EAST, WEST + NORTH_WEST, WEST + SOUTH_WEST,
});

inline constexpr std::array<bitboardType, SQUARE_COUNT> KING_ATTACKS = detail::leaperAttacks(std::array{
    NORTH, SOUTH, EAST, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST,
});

inline constexpr std::array<std::array<bitboardType, SQUARE_COUNT>, COLOR_COUNT> PAWN_ATTACKS = {
    detail::leaperAttacks(std::array{NORTH_EAST, NORTH_WEST}),
    detail::leaperAttacks(std::array{SOUTH_EAST, SOUTH_WEST}),
};

template <Direction D>
inline constexpr std::array<bitboardType, SQUARE_COUNT> RAY = [] {
    std::array<bitboardType, SQUARE_COUNT> table{};

    for (Square from = 0; from < SQUARE_COUNT; ++from)
        for (Square sq = step(from, D); sq != NO_SQUARE; sq = step(sq, D))
            setBit(table[from], sq);
    return table;
}();

template <Direction D>
[[nodiscard]] constexpr bitboardType rayAttacks(Square sq, bitboardType occupied)
{
    const bitboardType blockers = RAY<D>[sq] & occupied;

    if (blockers == 0)
        return RAY<D>[sq];

    const Square nearest = D.offset() > 0 ? std::countr_zero(blockers) : 63 - std::countl_zero(blockers);
    return RAY<D>[sq] ^ RAY<D>[nearest];
}

template <Direction... Ds>
[[nodiscard]] constexpr bitboardType slidingAttacks(Square sq, bitboardType occupied)
{
    return (rayAttacks<Ds>(sq, occupied) | ...);
}

template <PieceType Pt>
[[nodiscard]] constexpr bitboardType attacks(Square sq, [[maybe_unused]] bitboardType occupied)
{
    static_assert(Pt != PAWN, "pawn attacks depend on the color: use PAWN_ATTACKS");

    if constexpr (Pt == KNIGHT)
        return KNIGHT_ATTACKS[sq];
    else if constexpr (Pt == BISHOP)
        return slidingAttacks<NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST>(sq, occupied);
    else if constexpr (Pt == ROOK)
        return slidingAttacks<NORTH, SOUTH, EAST, WEST>(sq, occupied);
    else if constexpr (Pt == QUEEN)
        return attacks<BISHOP>(sq, occupied) | attacks<ROOK>(sq, occupied);
    else
        return KING_ATTACKS[sq];
}

} // namespace engine::movegen

#endif /* ATTACKS_HPP_ */
