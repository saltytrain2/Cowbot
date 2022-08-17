#pragma once

#include <cstdint>
#include <cctype>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <memory>

#include "types.h"
#include "Move.h"
#include "Attack.h"
#include "PRNG.h"

namespace Cowbot {
class ChessBoard
{
public:
    // Alternate constructor w/ default parameter
    // layout follows Forsyth-Edwards Notation, no guarantee on 3-fold repetition
    explicit ChessBoard(Attack* ptr, const std::string& layout = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    ~ChessBoard() = default;
    ChessBoard(const ChessBoard& rhs) = default;
    ChessBoard& operator=(const ChessBoard& rhs) = default;

    void updateChessBoard(const std::string& layout);

    // All of the get methods for each bitboard
    Bitboard getWhitePawns() const noexcept;
    Bitboard getWhiteKnights() const noexcept;
    Bitboard getWhiteBishops() const noexcept;
    Bitboard getWhiteRooks() const noexcept;
    Bitboard getWhiteQueens() const noexcept;
    Bitboard getWhiteKing() const noexcept;
    Bitboard getBlackPawns() const noexcept;
    Bitboard getBlackKnights() const noexcept;
    Bitboard getBlackBishops() const noexcept;
    Bitboard getBlackRooks() const noexcept;
    Bitboard getBlackQueens() const noexcept;
    Bitboard getBlackKing() const noexcept;
    Bitboard getWhitePieces() const noexcept;
    Bitboard getBlackPieces() const noexcept;
    Bitboard getAllPieces() const noexcept;
    Bitboard getEmptySquares() const noexcept;

    Bitboard getPawns(Color color) const noexcept;
    Bitboard getKnights(Color color) const noexcept;
    Bitboard getBishops(Color color) const noexcept;
    Bitboard getRooks(Color color) const noexcept;
    Bitboard getQueens(Color color) const noexcept;
    Bitboard getKing(Color color) const noexcept;

    Bitboard getPieces(Color color) const noexcept;
    Bitboard getPieces(PieceSets piece) const noexcept;
    PieceSets getPiece(Square sq) const noexcept;

    ZobristHash getHash() const noexcept;


    // get method for the turn
    Color getTurn() const noexcept;

    // get methods for castling rights
    bool getWhiteCastleRights(Castling side) const noexcept;
    bool getBlackCastleRights(Castling side) const noexcept;
    bool getCastleRights(Castling side, Color color) const noexcept;

    Move getLastMove() const noexcept;
    Bitboard getPinnedPieces() const noexcept;
    Bitboard getEnpassantTarget() const noexcept;
    void printSquareBoard() const noexcept;

    // makes the move
    void makeMove(const Move& nextMove);
    void undoMove();


    bool isLegal(const Move& move);
    bool isPseudoLegal(Move move) const;
    Bitboard getKingAttackers(Color turn, Bitboard blockers) const;
    bool isKingUnderAttack(Color turn, Bitboard blockers) const;
    bool isKingUnderAttack() const;
    bool isCapture(const Move& move) const;

    bool containsPromotingPawns() const;

    // debug
    bool validateSquareBoard() const;
private:
    Attack* mAttack;
    // buffer to store all bitboards describing the board state
    // boards 0-5 describe individual white pieces ordered from pawn, knight ... queen, king
    // boards 6-11 describe individual black pieces ordered from pawn, knight ... queen, king
    // board 12 describes all white pieces, board 13 describes all black pieces
    // board 14 describes all occupied squares, board 16 describes all empty squares
    Bitboard mPieceBB[15];
    // which player's turn it is to make a move
    Color mTurn;
 
    // buffers to check whehter the player can castle or not
    // index 0 for kingside, index 1 for queenside
    std::array<bool, 4> mCastle;


    // A square-centric represenation of the board
    PieceSets mSquareBoard[64];

    // keeping track of which pieces are currently pinned
    Bitboard mPinnedPieces;
    Bitboard mEnpassantTarget;

    // Zobrist Hashing Values of Position
    struct Zobrist {
        uint64_t mPSQ[12][64];
        uint64_t mEnpassant[8];
        uint64_t mCastling[4];
        uint64_t mTurn;
    };
    Zobrist mZobristTable;
    ZobristHash mZobristHash;

    // Struct that encapulates the information of the current position
    //TODO
    struct BoardState {
        BoardState(Move move, std::array<bool, 4> castle, Bitboard pinned, Bitboard enpassant, uint64_t hash, PieceSets capturedPiece)
            : mMove(move), mCastle(castle), mPinned(pinned), mEnpassant(enpassant), mZobristHash(hash), mCapturedPiece(capturedPiece)
        {}
        Move mMove;
        std::array<bool, 4> mCastle;
        Bitboard mPinned;
        Bitboard mEnpassant;
        uint64_t mZobristHash;
        PieceSets mCapturedPiece;
        // uint8_t mHalfMove;
        // uint8_t mFullMove;
    };
    std::vector<BoardState> mStateHistory;
    // Half-move and Full-Move Counters
    // uint8_t mHalfMove;
    // uint8_t mFullMove;

    void updateRedundantBitboards() noexcept;
    void updateSquareBoard() noexcept;
    void updateCastlingRights() noexcept;
    void updatePinnedPieces(Color color) noexcept;

    // TODO 
    // possibly have to overload this function with a string and rank version
    void updateBitboards(const std::vector<std::string>& piecesByRank) noexcept;
    void initZobristHashes();
    void updateZobristHash();

    bool isLegalPinnedMove(Square startSquare, Square endSquare, Color turn) const;
    Bitboard squareAttackers(Square sq, Color color, Bitboard blockers) const;
    Bitboard squareBlockers(Square sq, Color color) const;
    bool isSquareUnderAttack(Square sq, Color color, Bitboard blockers) const;
};
} // namespace Cowbot