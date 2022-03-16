#pragma once

#include <cstdint>
#include <cctype>
#include <string>
#include <sstream>
#include <iostream>
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
    inline Bitboard getWhitePawns() const noexcept {return mPieceBB[whitePawns];}
    inline Bitboard getWhiteKnights() const noexcept {return mPieceBB[whiteKnights];}
    inline Bitboard getWhiteBishops() const noexcept {return mPieceBB[whiteBishops];}
    inline Bitboard getWhiteRooks() const noexcept {return mPieceBB[whiteRooks];}
    inline Bitboard getWhiteQueens() const noexcept {return mPieceBB[whiteQueens];}
    inline Bitboard getWhiteKing() const noexcept {return mPieceBB[whiteKing];}
    inline Bitboard getBlackPawns() const noexcept {return mPieceBB[blackPawns];}
    inline Bitboard getBlackKnights() const noexcept {return mPieceBB[blackKnights];}
    inline Bitboard getBlackBishops() const noexcept {return mPieceBB[blackBishops];}
    inline Bitboard getBlackRooks() const noexcept {return mPieceBB[blackRooks];}
    inline Bitboard getBlackQueens() const noexcept {return mPieceBB[blackQueens];}
    inline Bitboard getBlackKing() const noexcept {return mPieceBB[blackKing];}
    inline Bitboard getWhitePieces() const noexcept {return mPieceBB[whitePieces];}
    inline Bitboard getBlackPieces() const noexcept {return mPieceBB[blackPieces];}
    inline Bitboard getAllPieces() const noexcept {return mPieceBB[allPieces];}
    inline Bitboard getEmptySquares() const noexcept {return mPieceBB[emptySquares];}

    // get method for the turn
    inline Bitboard getTurn() const noexcept {return mTurn;}

    // get methods for castling rights
    inline bool getWhiteCastleRights(Castling side) const noexcept {return mWhiteCastle[side];}
    inline bool getBlackCastleRights(Castling side) const noexcept {return mBlackCastle[side];}

    inline std::vector<Bitmove> getMoveList() const noexcept {return mMoveList;}
    void print88Board() const noexcept;

    // makes the move
    int makeMove(const Move& nextMove) noexcept;

    // TODO 
    // possibly have to overload this function with a string and rank version
    void updateBitboards(const std::vector<std::string>& piecesByRank) noexcept;

    void updateRedundantBitboards() noexcept;
    void update88Board() noexcept;



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

    // A square-centric represenation of the board
    PieceSets m88Board[64];

    std::vector<Bitmove> mMoveList;
};