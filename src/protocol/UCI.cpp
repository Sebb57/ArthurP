/**
 * @file UCI.cpp
 * @brief file implementing uci protocol
 *
 * @author Your Name
 * @date 2026-07-14
**/

#include "Protocol.hpp"
#include <iostream>
#include <sstream>

protocol::UCI::UCI()
{
    this->protocolMap["debug"] = [this](std::string& arg) {
        if (arg == "on")
            this->debug = true;
        else
            this->debug = false;
    };
    this->protocolMap["isready"] = [](std::string&) { std::cout << "readyok" << std::endl; };
    this->protocolMap["setoption"] = this->setoption;
    this->protocolMap["ucinewgame"] = this->ucinewgame; 
    this->protocolMap["position"] = this->position;
    this->protocolMap["go"] = this->go;
    this->protocolMap["quit"] = [this](std::string&) { this->running = false; };

    std::cout << "id name ArthurP" << std::endl;
    std::cout << "id author Sébastien Doussang" << std::endl;
    std::cout << "uciok" << std::endl;
}

void protocol::UCI::setoption(std::string& args)
{
    (void)args;
    //TODO: call engine and let him parse/set options
//  this->engine.setOption(args);
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
    } else {
        return;
    }

    while (iss >> token) {
        //TODO: store moves
    }
    //TODO: give position and moves to engine
}

void protocol::UCI::go(std::string& args)
{
    (void)args;
    //TODO: call engine go and let him do it 
//  engine.go(args);
}

void protocol::UCI::stop(std::string& args)
{
    (void)args;
    //TODO: call engine go and let him do it 
//  engine.stopCalculating();
//  std::cout << "bestmove " this->engine.getBestMove();
//  if (this->ponderEnabled) {
//      std::cout << " ponder " << engine.getPonder();
//  }
//  std::cout << std::endl;
}

void protocol::UCI::ponderhit(std::string& args)
{
    (void)args;
    //TODO: call engine to get the best moved based on ponder 
//  std::cout << "bestmove " << this->engine.getBestMovePonder();
//  if (this->ponderEnabled) {
//      std::cout << " ponder " << engine.getPonder();
// }
// std::cout << std::endl;
}
