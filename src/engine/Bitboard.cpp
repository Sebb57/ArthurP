/**
 * @file Bitboard.cpp
 * @brief bitboard methods
 *
 * @author Seb
 * @date 2026-07-16
**/

#include "Bitboard.hpp"
#include "utils/utils.hpp"
#include <algorithm>
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

constexpr engine::Piece promotionToPiece(engine::Piece moving, engine::piecePromotion promotion)
{
    switch (promotion) {
        case engine::QUEEN:  return moving == engine::WP ? engine::WQ : engine::BQ;
        case engine::ROOK:   return moving == engine::WP ? engine::WR : engine::BR;
        case engine::BISHOP: return moving == engine::WP ? engine::WB : engine::BB;
        case engine::KNIGHT: return moving == engine::WP ? engine::WN : engine::BN;
        default:             return moving;
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
    int from     = squareToBit(move.from);
    int to       = squareToBit(move.to);
    Piece moving = board[from];

    if (moving == EMPTY)
        return;

    clearBit(pieces[moving], from);

    if (move.castle) {
        int rookFrom = -1;
        int rookTo   = -1;
        Piece rook   = moving == WK ? WR : BR;

        if (to > from) {
            rookFrom = moving == WK ? squareToBit("h1") : squareToBit("h8");
            rookTo   = moving == WK ? squareToBit("f1") : squareToBit("f8");
        } else {
            rookFrom = moving == WK ? squareToBit("a1") : squareToBit("a8");
            rookTo   = moving == WK ? squareToBit("d1") : squareToBit("d8");
        }

        clearBit(pieces[rook], rookFrom);
        setBit(pieces[rook], rookTo);
        board[rookTo] = rook;
        board[rookFrom] = EMPTY;
    }

    if (move.capture)
        clearBit(pieces[board[to]], to);

    Piece placedPiece = moving;
    if (move.promotion)
        placedPiece = promotionToPiece(moving, move.promotionPiece);

    setBit(pieces[placedPiece], to);

    board[to] = placedPiece;
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
