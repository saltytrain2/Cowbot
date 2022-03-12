#pragma once

#include <cstdint>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include "types.h"
#include "Move.h"

class ChessBoard
{
public:
    // Alternate constructor w/ default parameter
    // layout follows Forsyth-Edwards Notation, no guarantee on 3-fold repetition
    explicit ChessBoard(std::string layout = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    ~ChessBoard() = default;
    ChessBoard(const ChessBoard& rhs) = default;
    ChessBoard& operator=(const ChessBoard& rhs) = default;

    // All of the get methods for each bitboard
    Bitboard getWhitePawns() const noexcept;
    Bitboard getWhiteKnights() const noexcept;
    Bitboard getWhiteBishops() const noexcept;
    Bitboard getWhiteRooks() const noexcept;
    Bitboard getWhiteQueens() const noexcept;
    Bitboard getWhiteKing() const noexcept;
    Bitboard getBlackPawns() const noexcept;
    Bitboard getBlackBishops() const noexcept;
    Bitboard getBlackKnights() const noexcept;
    Bitboard getBlackRooks() const noexcept;
    Bitboard getBlackQueens() const noexcept;
    Bitboard getBlackKing() const noexcept;
    Bitboard getWhitePieces() const noexcept;
    Bitboard getBlackPieces() const noexcept;
    Bitboard getAllPieces() const noexcept;
    Bitboard getAllEmptySquares() const noexcept;

    // get method for the turn
    Bitboard getTurn() const noexcept;

    // get methods for castling rights
    bool getWhiteCastleRights(Castling side) const noexcept;
    bool getBlackCastleRights(Castling side) const noexcept;

    void makeMove(Move nextMove);

    // TODO 
    // possibly have to overload this function with a string and rank version
    void setBitboards(const std::vector<std::string>& piecesByRank);



private:
    // buffer to store all bitboards describing the board state
    // boards 0-5 describe individual white pieces ordered from pawn, knight ... queen, king
    // boards 6-11 describe individual black pieces ordered from pawn, knight ... queen, king
    // board 12 describes all white pieces, board 13 describes all black pieces
    // board 14 describes all occupied squares, board 16 describes all empty squares
    Bitboard mPieceBB[16];
    // which player's turn it is to make a move
    Color mTurn;
 
    // buffers to check whehter the player can castle or not
    // index 0 for kingside, index 1 for queenside
    bool mWhiteCastle[2];
    bool mBlackCastle[2];
};