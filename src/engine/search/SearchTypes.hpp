#ifndef SEARCHTYPES_HPP_
#define SEARCHTYPES_HPP_

#include "engine/Types.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <optional>

namespace engine {

inline constexpr int MAX_DEPTH = 64;

struct SearchLimits {
    std::array<std::optional<std::int64_t>, COLOR_COUNT> time{};
    std::array<std::int64_t, COLOR_COUNT> increment{};
    std::optional<int> movesToGo;
    std::optional<std::int64_t> moveTime;
    std::optional<int> depth;
    std::optional<std::uint64_t> nodes;
    bool infinite = false;
};

struct SearchInfo {
    int depth = 0;
    int score = 0;
    std::optional<int> mateIn;
    std::uint64_t nodes = 0;
    std::int64_t timeMs = 0;
    Move bestMove;
};

using InfoCallback = std::function<void(const SearchInfo&)>;

} // namespace engine

#endif /* SEARCHTYPES_HPP_ */
