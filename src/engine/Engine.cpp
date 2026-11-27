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
#include "utils/Logger.hpp"
#include <string>
#include <vector>
#include <limits>

std::vector<engine::Move> engine::Engine::getLegalMoves() const
{
    return movegen::generateMoves(this->board);
}

std::string engine::Engine::search(int n) const
{
    (void)n;
    auto& logger = utils::Logger::get();
    const std::vector<Move> moves = this->getLegalMoves();
    std::string bestMove = "";
    int bestMoveScore = std::numeric_limits<int>::min();

    logger.debug("search(depth={}): {} legal moves, initial bestMoveScore={}", n, moves.size(), bestMoveScore);

    if (moves.empty()) {
        logger.warn("search: no legal moves");
        return "";
    }
    for (const auto& move : moves) {
        int current = this->eval.evaluate(this->board, move);
        logger.debug("  {} -> {}", moveToText(move), current);
        if (current > bestMoveScore) {
            bestMoveScore = current;
            bestMove = moveToText(move);
            logger.debug("    new best: {} ({})", bestMove, bestMoveScore);
        }
    }
    logger.info("search: bestmove={} score={}", bestMove, bestMoveScore);
    return bestMove;
}
