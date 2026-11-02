/**
 * @file MoveGenerator.hpp
 * @brief generates every move of the side to move from a bitboard
 *
 * @author Seb
 * @date 2026-07-21
**/

#ifndef MOVEGENERATOR_HPP_
#define MOVEGENERATOR_HPP_

#include "engine/Bitboard.hpp"
#include "engine/Types.hpp"
#include "engine/moveGeneration/GenFlags.hpp"
#include <vector>

namespace engine::movegen {

template <GenFlags Flags = GenFlags{}>
[[nodiscard]] std::vector<Move> generateMoves(const Bitboard& board);
[[nodiscard]] bool isSquareAttacked(const Bitboard& board, Square sq, Color by);
[[nodiscard]] bool isInCheck(const Bitboard& board, Color color);

} // namespace engine::movegen

#endif /* MOVEGENERATOR_HPP_ */
