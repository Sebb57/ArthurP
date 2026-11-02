/**
 * @file utils.hpp
 * @brief helper functions
 *
 * @author Seb
 * @date 2026-07-20
**/

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <vector>
#include <string>

namespace utils {

class Helpers {
    public:
        static std::vector<std::string> splitString(const std::string& input, std::string_view delimiter);
};

} // namespace utils

#endif /* UTILS_HPP_ */

