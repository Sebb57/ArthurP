/**
 * @file Bitboard.cpp
 * @brief bitboard methods
 *
 * @author Seb
 * @date 2026-07-16
**/

#include "engine/Bitboard.hpp"
#include "engine/Types.hpp"
#include <array>
#include <cctype>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

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

constexpr engine::CastlingRights fenToCastlingRight(char c)
{
    switch (c) {
        case 'K': return engine::WHITE_KINGSIDE;
        case 'Q': return engine::WHITE_QUEENSIDE;
        case 'k': return engine::BLACK_KINGSIDE;
        case 'q': return engine::BLACK_QUEENSIDE;
        default:  return engine::NO_CASTLING;
    }
}

} // namespace

engine::Bitboard::Bitboard()
{
    loadFEN(START_FEN);
}

void engine::Bitboard::clear()
{
    pieces.fill(0);
    colorPieces.fill(0);
    occupied = 0;
    board.fill(EMPTY);
    sideToMove = WHITE;
    castlingRights = NO_CASTLING;
    enPassantSquare = NO_SQUARE;
}

void engine::Bitboard::putPiece(Piece piece, Square sq)
{
    board[sq] = piece;
    setBit(pieces[piece], sq);
    setBit(colorPieces[pieceColor(piece)], sq);
    setBit(occupied, sq);
}

void engine::Bitboard::removePiece(Square sq)
{
    const Piece piece = board[sq];

    if (piece == EMPTY)
        return;
    board[sq] = EMPTY;
    clearBit(pieces[piece], sq);
    clearBit(colorPieces[pieceColor(piece)], sq);
    clearBit(occupied, sq);
}

void engine::Bitboard::movePiece(Square from, Square to)
{
    const Piece piece = board[from];

    if (piece == EMPTY)
        return;
    removePiece(from);
    putPiece(piece, to);
}

void engine::Bitboard::makeMove(const Move& move)
{
    const Piece moving = board[move.from];
    if (moving == EMPTY)
        return;

    if (move.enPassant)
        removePiece(makeSquare(fileOf(move.to), rankOf(move.from)));
    else
        removePiece(move.to);

    movePiece(move.from, move.to);

    if (move.promotion) {
        removePiece(move.to);
        putPiece(makePiece(sideToMove, move.promotionPiece), move.to);
    }

    if (move.castle) {
        for (const Castling& castling : CASTLINGS[sideToMove])
            if (castling.kingTo == move.to)
                movePiece(castling.rookFrom, castling.rookTo);
    }

    castlingRights &= ~(castlingRightsLost(move.from) | castlingRightsLost(move.to));

    enPassantSquare = NO_SQUARE;
    if (pieceType(moving) == PAWN && std::abs(move.to - move.from) == 16)
        enPassantSquare = (move.from + move.to) / 2;

    sideToMove = ~sideToMove;
}

void engine::Bitboard::loadFEN(std::string_view fen)
{
    std::istringstream iss{std::string{fen}};
    std::string placement;
    std::string side = "w";
    std::string castling = "-";
    std::string enPassant = "-";
    int file = 0;
    int rank = 7;

    iss >> placement >> side >> castling >> enPassant;
    clear();
    for (const char c : placement) {
        if (c == '/') {
            --rank;
            file = 0;
        } else if (std::isdigit(static_cast<unsigned char>(c)) != 0) {
            file += c - '0';
        } else {
            const Piece piece = fenToPiece(c);
            if (piece != EMPTY && isOnBoard(file, rank))
                putPiece(piece, makeSquare(file, rank));
            ++file;
        }
    }

    sideToMove = side == "b" ? BLACK : WHITE;

    for (const char c : castling)
        castlingRights |= fenToCastlingRight(c);
    for (const Color color : {WHITE, BLACK})
        for (const Castling& right : CASTLINGS[color])
            if (board[right.kingFrom] != makePiece(color, KING) || board[right.rookFrom] != makePiece(color, ROOK))
                castlingRights &= ~right.right;

    if (enPassant.size() == 2 && isOnBoard(enPassant[0] - 'a', enPassant[1] - '1'))
        enPassantSquare = squareToBit(enPassant);
}

void engine::Bitboard::printBoard() const
{
    static constexpr std::array<char, PIECE_COUNT> pieceChar = {
        '.', // EMPTY
        'P', 'N', 'B', 'R', 'Q', 'K',
        'p', 'n', 'b', 'r', 'q', 'k',
    };
    static constexpr std::array<std::string_view, PIECE_COUNT> pieceNames = {
        "EMPTY",
        "WP", "WN", "WB", "WR", "WQ", "WK",
        "BP", "BN", "BB", "BR", "BQ", "BK",
    };

    std::cout << "=== Board ===" << std::endl;
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " ";
        for (int file = 0; file < 8; ++file)
            std::cout << pieceChar[board[makeSquare(file, rank)]] << ' ';
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
    for (int p = WP; p <= BK; ++p) {
        std::cout << pieceNames[p] << ":" << std::endl;
        for (int rank = 7; rank >= 0; --rank) {
            std::cout << rank + 1 << " ";
            for (int file = 0; file < 8; ++file)
                std::cout << (getBit(pieces[p], makeSquare(file, rank)) ? "1 " : ". ");
            std::cout << std::endl;
        }
        std::cout << "  a b c d e f g h" << std::endl << std::endl;
    }
}
