/**
 * @file UCI.cpp
 * @brief file implementing uci protocol
 *
 * @author Seb
 * @date 2026-07-14
**/

#include "protocol/UCI.hpp"
#include "engine/Engine.hpp"
#include "engine/Types.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

void send(const std::string& line)
{
    static std::mutex outputMutex;
    const std::scoped_lock lock(outputMutex);

    std::cout << line << std::endl;
}

template <typename T>
T readNumber(std::istringstream& iss)
{
    T value{};

    iss >> value;
    return value;
}

engine::SearchLimits parseGo(const std::string& args)
{
    engine::SearchLimits limits;
    std::string token;
    std::istringstream iss(args);

    while (iss >> token) {
        if (token == "wtime")
            limits.time[engine::WHITE] = readNumber<std::int64_t>(iss);
        else if (token == "btime")
            limits.time[engine::BLACK] = readNumber<std::int64_t>(iss);
        else if (token == "winc")
            limits.increment[engine::WHITE] = readNumber<std::int64_t>(iss);
        else if (token == "binc")
            limits.increment[engine::BLACK] = readNumber<std::int64_t>(iss);
        else if (token == "movestogo")
            limits.movesToGo = readNumber<int>(iss);
        else if (token == "movetime")
            limits.moveTime = readNumber<std::int64_t>(iss);
        else if (token == "depth")
            limits.depth = readNumber<int>(iss);
        else if (token == "nodes")
            limits.nodes = readNumber<std::uint64_t>(iss);
        else if (token == "infinite" || token == "ponder") // no pondering yet so will think until stop
            limits.infinite = true;
    }
    if (!limits.time[engine::WHITE] && !limits.time[engine::BLACK] && !limits.moveTime && !limits.depth && !limits.nodes)
        limits.infinite = true;
    return limits;
}

void sendInfo(const engine::SearchInfo& info)
{
    const std::string score = info.mateIn ? "mate " + std::to_string(*info.mateIn) : "cp " + std::to_string(info.score);
    const std::uint64_t nps = info.timeMs > 0 ? info.nodes * 1000 / static_cast<std::uint64_t>(info.timeMs) : 0;

    send("info depth " + std::to_string(info.depth) + " score " + score + " nodes " + std::to_string(info.nodes) + " nps " + std::to_string(nps) + " time " + std::to_string(info.timeMs) + " pv " + engine::moveToText(info.bestMove));
}

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
    protocolMap["stop"] = bindMember<UCI, &UCI::stop>(this);
    protocolMap["quit"] = bindMember<UCI, &UCI::quit>(this);

    protocolMap["isready"] = [](std::string&) { send("readyok"); };

    send("id name ArthurP");
    send("id author Sébastien Doussang");
    send("uciok");
}

protocol::UCI::~UCI()
{
    this->engine.stop();
    this->waitForSearch();
}

void protocol::UCI::waitForSearch()
{
    if (this->searchThread.joinable())
        this->searchThread.join();
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
    this->waitForSearch();
    //TODO: clear engine position and reasoning
//  this->engine.clear();
}

void protocol::UCI::position(std::string& args)
{
    std::string token;
    std::istringstream iss(args);

    this->waitForSearch();
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
    const engine::SearchLimits limits = parseGo(args);

    this->waitForSearch();
    this->engine.clearStop();
    this->searchThread = std::jthread([this, limits] {
        const std::string bestMove = this->engine.search(limits, sendInfo);
        send(bestMove.empty() ? "bestmove" : "bestmove " + bestMove);
    });
}

void protocol::UCI::stop(std::string& args)
{
    (void)args;
    this->engine.stop();
}

void protocol::UCI::quit(std::string& args)
{
    (void)args;
    this->engine.stop();
    this->waitForSearch();
    this->running = false;
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
