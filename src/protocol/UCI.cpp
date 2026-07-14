/**
 * @file UCI.cpp
 * @brief file implementing uci protocol
 *
 * @author Your Name
 * @date 2026-07-14
**/

#include "Protocol.hpp"
#include <iostream>

protocol::UCI::UCI()
{
    this->protocolMap["quit"] = [this](std::string&) { this->running = false; };

    this->protocolMap["isready"] = [](std::string&) { std::cout << "readyok" << std::endl; };

    std::cout << "uciok" << std::endl;
}
