/**
 * @file UCI.cpp
 * @brief file implementing uci protocol
 *
 * @author Seb
 * @date 2026-07-14
**/

#include "Protocol.hpp"
#include "engine/Engine.hpp"
#include "engine/Types.hpp"
#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::optional<engine::Move> parseUciMove(std::string_view text, const engine::Engine& boardEngine)
{
    const std::vector<engine::Move> moves = boardEngine.getLegalMoves();
    const auto found = std::ranges::find(moves, text, engine::moveToText);

    if (found == moves.end())
        return std::nullopt;
    return *found;
}

} // namespace

protocol::UCI::UCI()
{
    protocolMap["debug"] = bindMember<UCI, &UCI::debug>(this);
    protocolMap["setoption"] = bindMember<UCI, &UCI::setoption>(this);
    protocolMap["ucinewgame"] = bindMember<UCI, &UCI::ucinewgame>(this);
    protocolMap["position"] = bindMember<UCI, &UCI::position>(this);
    protocolMap["go"] = bindMember<UCI, &UCI::go>(this);

    protocolMap["isready"] = [](std::string&) { std::cout << "readyok" << std::endl; };
    protocolMap["quit"] = [this](std::string&) { this->running = false; };

    std::cout << "id name ArthurP" << std::endl;
    std::cout << "id author Sébastien Doussang" << std::endl;
    std::cout << "uciok" << std::endl;
}

void protocol::UCI::debug(std::string& arg)
{
    if (arg == "on")
        this->debugRun = true;
    else
        this->debugRun = false;
}

void protocol::UCI::setoption(std::string& args)
{
    (void)args;
    //TODO: parse go and call corresponding engine function 
}

void protocol::UCI::ucinewgame(std::string&)
{
    //TODO: clear engine position and reasoning
//  this->engine.clear();
}

void protocol::UCI::position(std::string& args)
{
    std::string token;
    std::istringstream iss(args);

    iss >> token;
    if (token == "startpos") {
        this->engine.loadFen(engine::START_FEN);
        iss >> token; // "moves"
    } else if (token == "fen") {
        std::string fen;
        while (iss >> token && token != "moves") {
            fen += token;
            fen += ' ';
        }
        this->engine.loadFen(fen);
    } else {
        return;
    }

    while (iss >> token) {
        const std::optional<engine::Move> move = parseUciMove(token, this->engine);
        if (!move)
            return; // unknown or illegal move: keep the position reached so far
        this->engine.makeMove(*move);
    }
}

void protocol::UCI::go(std::string& args)
{
    (void)args;
    const std::string bestMove = this->engine.search(0);
    std::cout << "bestmove";
    if (!bestMove.empty())
        std::cout << ' ' << bestMove;
    std::cout << std::endl;
}

void protocol::UCI::stop(std::string& args)
{
    (void)args;
    //TODO: call engine go and let him do it 
//  engine.stopCalculating();
//  std::cout << "bestmove" this->engine.getBestMove();
//  if (this->ponderEnabled) {
//      std::cout << " ponder " << engine.getPonder();
//  }
//  std::cout << std::endl;
}

void protocol::UCI::ponderhit(std::string& args)
{
    (void)args;
    //TODO: call engine to get the best moved based on ponder 
//  std::cout << "bestmove" << this->engine.getBestMovePonder();
//  if (this->ponderEnabled) {
//      std::cout << " ponder " << engine.getPonder();
// }
// std::cout << std::endl;
}
