#ifndef SEARCH_HPP_
#define SEARCH_HPP_

#include "engine/Bitboard.hpp"
#include "engine/Types.hpp"
#include "eval/Eval.hpp"
#include <atomic>
#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>

namespace engine::search {

inline constexpr int INF = 1000000;
inline constexpr int MATE = 900000;
inline constexpr std::uint64_t CHECK_INTERVAL_MASK = 1023;

struct SearchContext {
    const eval::Evaluator* eval = nullptr;
    const std::atomic<bool>* stopRequested = nullptr;
    std::optional<std::chrono::steady_clock::time_point> deadline;
    std::optional<std::uint64_t> maxNodes;
    std::uint64_t nodes = 0;
    bool aborted = false;

    bool shouldAbort()
    {
        if (this->aborted)
            return true;
        if ((this->nodes & CHECK_INTERVAL_MASK) != 0)
            return false;
        this->aborted = this->stopRequested->load(std::memory_order_relaxed) || (this->maxNodes && this->nodes >= *this->maxNodes) || (this->deadline && std::chrono::steady_clock::now() >= *this->deadline);
        return this->aborted;
    }
};

[[nodiscard]] std::optional<Move> searchRoot(const Bitboard& board, const std::vector<Move>& moves, int depth, int& bestScore, SearchContext& ctx);
[[nodiscard]] std::optional<int> mateIn(int score);

} // namespace engine::search

#endif /* SEARCH_HPP_ */
