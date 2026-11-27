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
#include <algorithm>
#include <string>
#include <vector>

namespace {

constexpr int INF = 1000000;
constexpr int MATE = 900000;

int negamax(const engine::Bitboard& board, int depth, int alpha, int beta, int ply, const eval::Evaluator& eval)
{
    if (depth == 0)
        return eval.evaluate(board);

    const std::vector<engine::Move> moves = engine::movegen::generateMoves(board);
    if (moves.empty())
        return engine::movegen::isInCheck(board, board.getSideToMove()) ? -MATE + ply : 0;

    for (const auto& move : moves) {
        engine::Bitboard next = board;
        next.makeMove(move);
        const int score = -negamax(next, depth - 1, -beta, -alpha, ply + 1, eval);
        if (score >= beta)
            return score;
        alpha = std::max(alpha, score);
    }
    return alpha;
}

} // namespace

std::vector<engine::Move> engine::Engine::getLegalMoves() const
{
    return movegen::generateMoves(this->board);
}

std::string engine::Engine::search(int depth) const
{
    auto& logger = utils::Logger::get();
    const std::vector<Move> moves = this->getLegalMoves();
    std::string bestMove = "";
    int bestMoveScore = -INF;

    logger.debug("search(depth={}): {} legal moves", depth, moves.size());

    if (moves.empty()) {
        logger.warn("search: no legal moves");
        return "";
    }
    for (const auto& move : moves) {
        Bitboard next = this->board;
        next.makeMove(move);
        const int current = -negamax(next, std::max(depth - 1, 0), -INF, -bestMoveScore, 1, this->eval);
        logger.debug("  {} -> {}", moveToText(move), current);
        if (current > bestMoveScore || bestMove.empty()) {
            bestMoveScore = current;
            bestMove = moveToText(move);
            logger.debug("    new best: {} ({})", bestMove, bestMoveScore);
        }
    }
    logger.info("search: bestmove={} score={}", bestMove, bestMoveScore);
    return bestMove;
}
