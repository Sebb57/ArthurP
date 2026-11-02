/**
 * @file GenFlags.hpp
 * @brief selects which moves the move generator returns
 *
 * @author Seb
 * @date 2026-11-02
**/

#ifndef GENFLAGS_HPP_
#define GENFLAGS_HPP_

namespace engine::movegen {

struct GenFlags {
    bool legal = false;
    bool illegal = false;

    [[nodiscard]] constexpr bool includesLegal() const { return legal || !illegal; }
    [[nodiscard]] constexpr bool includesIllegal() const { return illegal; }
};

} // namespace engine::movegen

#endif /* GENFLAGS_HPP_ */
