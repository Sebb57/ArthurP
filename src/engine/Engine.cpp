/**
 * @file Engine.cpp
 * @brief engine algorithms to find best move
 *
 * @author Seb
 * @date 2026-07-21
**/

#include "engine/Engine.hpp"
#include "engine/Types.hpp"
#include "engine/moveGeneration/MoveGenerator.hpp"
#include <string>
#include <vector>

std::vector<engine::Move> engine::Engine::getLegalMoves() const
{
    return movegen::generateMoves(this->board);
}

std::string engine::Engine::search(int n) const
{
    (void)n;
    const std::vector<Move> moves = this->getLegalMoves();

    if (moves.empty())
        return "";
    return moveToText(moves.front());
}
