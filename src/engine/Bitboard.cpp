/**
 * @file Bitboard.cpp
 * @brief bitboard methods
 *
 * @author Seb
 * @date 2026-07-16
**/

#include "Bitboard.hpp"
#include "utils/utils.hpp"
#include <string>
#include <vector>
#include <iostream>

namespace {

constexpr engine::Piece fenToPiece(char c) {
    switch (c) {
        case 'P': return engine::WP;
        case 'N': return engine::WN;
        case 'B': return engine::WB;
        case 'R': return engine::WR;
        case 'Q': return engine::WQ;
        case 'K': return engine::WK;

        case 'p': return engine::BP;
        case 'n': return engine::BN;
        case 'b': return engine::BB;
        case 'r': return engine::BR;
        case 'q': return engine::BQ;
        case 'k': return engine::BK;

        default:  return engine::EMPTY;
    }
}

}

int engine::Bitboard::squareToBit(const std::string& square)
{
    char file     = square[0];
    char rank     = square[1];
    int fileIndex = file - 'a';
    int rankIndex = rank - '1';

    return rankIndex * 8 + fileIndex;
}

std::string engine::Bitboard::bitToSquare(int bit)
{
    char file = 'a' + (bit % 8);
    char rank = '1' + (bit / 8);

    return std::string{file, rank};
}

void engine::Bitboard::setBit(uint64_t& bb, int sq)
{
    bb |= 1ULL << sq;
}

void engine::Bitboard::clearBit(uint64_t& bb, int sq)
{
    bb &= ~(1ULL << sq);
}

bool engine::Bitboard::getBit(uint64_t bb, int sq)
{
    return bb & (1ULL << sq);
}

void engine::Bitboard::makeMove(const Move& move)
{
    //TODO: handle move->castle case, the move var will countain the king move and it should move the rook to
    int from     = squareToBit(move.from);
    int to       = squareToBit(move.to);
    Piece moving = board[from];

    if (moving == EMPTY)
        return;
    clearBit(pieces[moving], from);

    if (move.capture)
        clearBit(pieces[board[to]], to);
    setBit(pieces[moving], to);

    board[to] = moving;
    board[from] = EMPTY;
    whitePieces = pieces[WP] | pieces[WN] | pieces[WB] | pieces[WR] | pieces[WQ] | pieces[WK];
    blackPieces = pieces[BP] | pieces[BN] | pieces[BB] | pieces[BR] | pieces[BQ] | pieces[BK];
    occupied = whitePieces | blackPieces;
    empty = ~occupied;
}

void engine::Bitboard::loadFEN(const std::string& fen)
{
    std::vector<std::string> splitFen = utils::Helpers::splitString(fen, "/ ");
    char rank = '8';
    char file = 'a';

    std::fill(std::begin(pieces), std::end(pieces), 0ULL);
    std::fill(std::begin(board), std::end(board), EMPTY);
    for (const auto& fenElm : splitFen) {
        if (fenElm == "w" || fenElm == "b")
            break;
        file = 'a';
        for (char c : fenElm) {
            if (std::isdigit(c)) {
                file += c - '0';
                continue;
            }
            Piece piece = fenToPiece(c);
            if (piece == EMPTY)
                continue;
            std::string square{file, rank};
            int bit = squareToBit(square);
            board[bit] = piece;
            setBit(pieces[piece], bit);
            ++file;
        }
        --rank;
    }
    whitePieces = pieces[WP] | pieces[WN] | pieces[WB] | pieces[WR] | pieces[WQ] | pieces[WK];
    blackPieces = pieces[BP] | pieces[BN] | pieces[BB] | pieces[BR] | pieces[BQ] | pieces[BK];
    occupied = whitePieces | blackPieces;
    empty = ~occupied;
    // printBoard();
}

void engine::Bitboard::printBoard()
{
    static constexpr char pieceChar[] = {
        '.', // EMPTY
        'P', 'N', 'B', 'R', 'Q', 'K',
        'p', 'n', 'b', 'r', 'q', 'k'
    };
    static constexpr const char* pieceNames[] = {
        "EMPTY",
        "WP", "WN", "WB", "WR", "WQ", "WK",
        "BP", "BN", "BB", "BR", "BQ", "BK"
    };

    std::cout << "=== Board ===" << std::endl;
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " ";
        for (int file = 0; file < 8; ++file) {
            int sq = rank * 8 + file;
            std::cout << pieceChar[board[sq]] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
    for (int p = WP; p <= BK; ++p) {
        std::cout << pieceNames[p] << ":" << std::endl;
        for (int rank = 7; rank >= 0; --rank) {
            std::cout << rank + 1 << " ";
            for (int file = 0; file < 8; ++file) {
                int sq = rank * 8 + file;
                std::cout << (getBit(pieces[p], sq) ? "1 " : ". ");
            }
            std::cout << std::endl;
        }
        std::cout << "  a b c d e f g h" << std::endl << std::endl;
    }
}

