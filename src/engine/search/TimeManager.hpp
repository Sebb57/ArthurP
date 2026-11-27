#ifndef TIMEMANAGER_HPP_
#define TIMEMANAGER_HPP_

#include "engine/Types.hpp"
#include "engine/search/SearchTypes.hpp"
#include <cstdint>
#include <optional>

namespace engine::search {

struct TimeBudget {
    std::int64_t softMs;
    std::int64_t hardMs;
};

[[nodiscard]] std::optional<TimeBudget> computeBudget(const SearchLimits& limits, Color us);

} // namespace engine::search

#endif /* TIMEMANAGER_HPP_ */
