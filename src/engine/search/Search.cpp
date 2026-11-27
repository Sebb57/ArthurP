#include "engine/search/Search.hpp"
#include "engine/moveGeneration/MoveGenerator.hpp"
#include "engine/search/SearchTypes.hpp"
#include <algorithm>
#include <cstdlib>
#include <optional>
#include <vector>

namespace {

using engine::search::MATE;
using engine::search::SearchContext;

int negamax(const engine::Bitboard& board, int depth, int alpha, int beta, int ply, SearchContext& ctx)
{
    if (ctx.shouldAbort())
        return 0;
    ctx.nodes++;
    if (depth == 0)
        return ctx.eval->evaluate(board);

    const std::vector<engine::Move> moves = engine::movegen::generateMoves(board);
    if (moves.empty())
        return engine::movegen::isInCheck(board, board.getSideToMove()) ? -MATE + ply : 0;

    for (const auto& move : moves) {
        engine::Bitboard next = board;
        next.makeMove(move);
        const int score = -negamax(next, depth - 1, -beta, -alpha, ply + 1, ctx);
        if (ctx.aborted)
            return 0;
        if (score >= beta)
            return score;
        alpha = std::max(alpha, score);
    }
    return alpha;
}

} // namespace

std::optional<engine::Move> engine::search::searchRoot(const Bitboard& board, const std::vector<Move>& moves, int depth, int& bestScore, SearchContext& ctx)
{
    std::optional<Move> best;
    int alpha = -INF;

    for (const auto& move : moves) {
        Bitboard next = board;
        next.makeMove(move);
        const int score = -negamax(next, depth - 1, -INF, -alpha, 1, ctx);
        if (ctx.aborted)
            break;
        if (score > alpha) {
            alpha = score;
            best = move;
        }
    }
    if (best)
        bestScore = alpha;
    return best;
}

std::optional<int> engine::search::mateIn(int score)
{
    if (std::abs(score) < MATE - MAX_DEPTH)
        return std::nullopt;
    return score > 0 ? (MATE - score + 1) / 2 : -(MATE + score) / 2;
}
