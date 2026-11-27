#include "engine/search/TimeManager.hpp"
#include <algorithm>
#include <cstdint>
#include <optional>

namespace {

constexpr std::int64_t MOVE_OVERHEAD_MS = 30;
constexpr int DEFAULT_MOVES_TO_GO = 30;

} // namespace

std::optional<engine::search::TimeBudget> engine::search::computeBudget(const SearchLimits& limits, Color us)
{
    if (limits.moveTime) {
        const std::int64_t budget = std::max<std::int64_t>(*limits.moveTime - MOVE_OVERHEAD_MS, 1);
        return TimeBudget{.softMs = budget * 2, .hardMs = budget};
    }
    const std::optional<std::int64_t> clock = limits.time[us];
    if (!clock)
        return std::nullopt;

    const std::int64_t timeLeft = std::max<std::int64_t>(*clock, 0);
    const std::int64_t increment = limits.increment[us];
    const int movesToGo = std::max(limits.movesToGo.value_or(DEFAULT_MOVES_TO_GO), 1);
    const std::int64_t maxAllowed = std::max<std::int64_t>(timeLeft - MOVE_OVERHEAD_MS, 1);
    const std::int64_t target = timeLeft / movesToGo + increment * 3 / 4;

    return TimeBudget{.softMs = std::min(target, maxAllowed), .hardMs = std::min(target * 2, maxAllowed)};
}
