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
#include "engine/search/Search.hpp"
#include "engine/search/TimeManager.hpp"
#include "utils/Logger.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace {

std::int64_t elapsedMs(std::chrono::steady_clock::time_point start)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
}

} // namespace

std::vector<engine::Move> engine::Engine::getLegalMoves() const
{
    return movegen::generateMoves(this->board);
}

std::string engine::Engine::search(const SearchLimits& limits, const InfoCallback& onInfo) const
{
    auto& logger = utils::Logger::get();
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::vector<Move> moves = this->getLegalMoves();
    const std::optional<search::TimeBudget> budget = search::computeBudget(limits, this->board.getSideToMove());
    const int maxDepth = std::clamp(limits.depth.value_or(MAX_DEPTH), 1, MAX_DEPTH);
    search::SearchContext ctx{.eval = &this->eval, .stopRequested = &this->stopRequested, .deadline = std::nullopt, .maxNodes = limits.nodes,};

    if (budget && !limits.infinite)
        ctx.deadline = start + std::chrono::milliseconds(budget->hardMs);
    logger.debug("search: {} legal moves, max depth {}, budget {}ms", moves.size(), maxDepth, budget ? budget->hardMs : -1);

    if (moves.empty()) {
        logger.warn("search: no legal moves");
        return "";
    }

    Move bestMove = moves.front();
    int bestScore = -search::INF;
    for (int depth = 1; depth <= maxDepth; depth++) {
        if (const std::optional<Move> iterationBest = search::searchRoot(this->board, moves, depth, bestScore, ctx)) {
            bestMove = *iterationBest;
            const auto found = std::ranges::find(moves, bestMove);
            std::rotate(moves.begin(), found, found + 1);
        }
        if (ctx.aborted)
            break;

        const std::int64_t elapsed = elapsedMs(start);
        logger.debug("  depth {}: {} ({}) nodes={} time={}ms", depth, moveToText(bestMove), bestScore, ctx.nodes, elapsed);
        if (onInfo)
            onInfo(SearchInfo{.depth = depth, .score = bestScore, .mateIn = search::mateIn(bestScore), .nodes = ctx.nodes, .timeMs = elapsed, .bestMove = bestMove,});

        if (limits.infinite)
            continue;
        if (budget && (moves.size() == 1 || elapsed * 2 >= budget->softMs))
            break;
        if (search::mateIn(bestScore))
            break;
    }
    logger.info("search: bestmove={} score={} nodes={} time={}ms", moveToText(bestMove), bestScore, ctx.nodes, elapsedMs(start));
    return moveToText(bestMove);
}
