/**
 * @file UCI.cpp
 * @brief file implementing uci protocol
 *
 * @author Seb
 * @date 2026-07-14
**/

#include "Protocol.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <sstream>
#include <vector>

//TODO: place this inside protocol.hpp 
namespace {

template <typename T, auto Member>
auto bindMember(T* obj)
{
    return [obj](std::string& arg) {
        (obj->*Member)(arg);
    };
}

engine::Move parseUciMove(const std::string& text, const engine::Engine& boardEngine)
{
    engine::Move move;

    if (text.size() < 4)
        return move;

    move.from = text.substr(0, 2);
    move.to = text.substr(2, 2);

    if (text.size() == 5) {
        move.promotion = true;
        switch (text[4]) {
            case 'q': move.promotionPiece = engine::QUEEN; break;
            case 'r': move.promotionPiece = engine::ROOK; break;
            case 'b': move.promotionPiece = engine::BISHOP; break;
            case 'n': move.promotionPiece = engine::KNIGHT; break;
            default:  move.promotion = false; break;
        }
    }

    const engine::Piece movingPiece = boardEngine.getPieceAt(move.from);
    if ((movingPiece == engine::WK || movingPiece == engine::BK) &&
        ((move.from == "e1" && (move.to == "g1" || move.to == "c1")) ||
         (move.from == "e8" && (move.to == "g8" || move.to == "c8")))) {
        move.castle = true;
    }
    return move;
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
        this->whiteToMove = true;
        iss >> token;
    } else if (token == "fen") {
        std::string sideToMove = "w";
        size_t pos = token.find(' ');

        if (pos != std::string::npos)
            sideToMove = token[pos + 1];
        this->whiteToMove = (sideToMove == "w");
        this->engine.loadFen(fen);
    } else {
        return;
    }

    while (iss >> token) {
        engine::Move move = parseUciMove(token, this->engine);
        this->engine.makeMove(move);
        this->whiteToMove = !this->whiteToMove;
    }
}

void protocol::UCI::go(std::string& args)
{
    (void)args;
    const std::string bestMove = this->engine.search(0, this->whiteToMove);
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
