/**
 * @file UCI.cpp
 * @brief file implementing uci protocol
 *
 * @author Seb
 * @date 2026-07-14
**/

#include "Protocol.hpp"
#include <iostream>
#include <sstream>

//TODO: place this inside protocol.hpp 
namespace {

template <typename T, auto Member>
auto bindMember(T* obj)
{
    return [obj](std::string& arg) {
        (obj->*Member)(arg);
    };
}

}

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
    std::string token, fen;
    std::istringstream iss(args);

    iss >> token;
    if (token == "startpos") {
        fen = protocol::StartFEN;
        iss >> token;
    } else if (token == "fen") {
        while (iss >> token && token != "moves")
            fen += token + " ";
        this->engine.loadFen(fen); // board default fen is StartFEN
    } else {
        return;
    }

    while (iss >> token) {
        //TODO: store moves
    }
}

void protocol::UCI::go(std::string& args)
{
    (void)args;
    //TODO: parse go and call corresponding engine function 
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
