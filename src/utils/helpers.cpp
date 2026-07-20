/**
 * @file helpers.cpp
 * @brief helper functions
 *
 * @author Seb
 * @date 2026-07-20
**/

#include "utils.hpp"
#include <string>
#include <vector>

std::vector<std::string> utils::Helpers::splitString(const std::string& input, std::string_view delimiters)
{
    std::vector<std::string> result;
    std::string token;

    for (auto& letter : input) {
        if (delimiters.find(letter) != std::string::npos) {
            result.push_back(token);
            token = "";
        } else {
            token += letter;
        } 
    }

    return result;
}
